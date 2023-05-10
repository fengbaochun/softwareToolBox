#ifndef THREADPOOL_H
#define THREADPOOL_H

#include <functional>
#include <iostream>
#include <vector>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <cstring>
#include <chrono>
#include <map>
#include <mutex>

using namespace std;

/// 对任务进行封装， 线程池中见到的就是一个个任务类
class Task {
public:
    /// 构造函数  将传入的具体执行函数 与 参数  打包成一个处理任务，这里需要使用 可变参函数模板
    ///参看 https://www.cnblogs.com/qicosmos/p/4325949.html
    ///
    template<typename Func_T, typename ...ARGS>
    explicit Task(Func_T func, ARGS ...args) {
        /// 将 传入的 函数 参数 bind 到 function实例
        /// std::forward 保持类型信息， 参见 https://blog.csdn.net/coolwriter/article/details/80970718
        this->func = std::bind(func, std::forward<ARGS>(args)...);
        /// this->func = std::bind(func, args...);
    }

    void run() {
        this->func();
    }

    /// 类模版std::function是一种通用、多态的函数封装  参见 https://blog.csdn.net/hzy925/article/details/79676085
    /// std::function的实例可以对任何可以调用的目标实体进行存储、复制、和调用操作，这些目标实体包括普通函数、Lambda表达式、函数指针、以及其它函数对象等。
    /// 这里实例 目标实体 是个 函数
    std::function<void()> func;
};


/// 线程池
template <typename QueueType = std::queue<Task *>>
class ThreadPool {
public:
    /// 构造函数 创建 n 个线程
    explicit ThreadPool(size_t n){
        std::cout << "ThreadPool start" <<std:: endl;
        timeMap[std::this_thread::get_id()] = clock();

        is_running = true;
        for (int i = 0; i < n; ++i) {
            thread *thd = new std::thread(&ThreadPool::thread_worker,this,i);
            threads.push_back(thd);
        }

    }
    /// 线程销毁
    ~ThreadPool() {

        {// 将 lock 做成局部变量，出了作用域以后，互斥锁就会被自动释放,不然 线程不能正常退出
            std::unique_lock<std::mutex> lock(m_mutex);
            is_running = false;
            /// notify_all 使用 参见 https://blog.csdn.net/feikudai8460/article/details/109604690
            /// 先锁住然后唤醒所有等待队列中阻塞的线程， 所有 m_cond.wait(lock); 会尝试获得锁
            m_cond.notify_all();
        }

        for (auto & thread : threads) {
            thread->join();
            delete thread;
        }
        timeMap[std::this_thread::get_id()] = float(clock( ) - timeMap[std::this_thread::get_id()]);
        viewPoolInfo();

        std::cout << u8"ThreadPool end" << " use time: "<< timeMap[std::this_thread::get_id()] <<std:: endl;        
    }

    // 工作线程入口函数 thread_worker
    /// 取任务和执行任务之间做循环，直到整个线程池停止工作、被销毁为止。
    void thread_worker(int i){
        threadInfoType temp;
        temp.index = i;
        temp.id = std::this_thread::get_id();
        while (is_running) {
            Task *t = this->getOneTask();
            if (t == nullptr)
                break;
            t->run();
        }
        ///计算子线程耗时
        timeMap[std::this_thread::get_id()] = float(clock( ) - timeMap[std::this_thread::get_id()]);
        temp.useTime = timeMap[std::this_thread::get_id()];
        info.push_back(temp);
        std::cout << "worker "<< i <<" end" << " use time: "<< timeMap[std::this_thread::get_id()]<<"ms" <<std:: endl;
   }

   /// 从队列中 获取任务
    Task *getOneTask() {
        /// unique_lock与lock_guard区别 参看 https://www.cnblogs.com/xudong-bupt/p/9194394.html
       /// unique_lock 使用 参看 https://blog.csdn.net/u012507022/article/details/85909567
       std::unique_lock<std::mutex> lock(m_mutex);
       /// 先锁住然后等待条件变量
       while (is_running && tasks.empty())
           m_cond.wait(lock);
       Task *t = nullptr;
       /// 取出一个任务来执行
       if (is_running) {
           timeMap[std::this_thread::get_id()] = clock();                  //线程和时间绑定
           t = tasks.front();
           tasks.pop();
       }
       return t;
    }

    /// 添加任务到 队列
    void addOneTask(Task *t) {
        std::unique_lock<std::mutex> lock(m_mutex);
        tasks.push(t);
        /// 通知 工作线程 有任务来了
        m_cond.notify_one();
    }
    ///显示线程池信息
    void viewPoolInfo(){
        std::cout <<"****************************************************************"<<std:: endl;
        for(int i =0;i<info.size();i++){
            std::cout << "taskId : "<< info[i].index <<" threadId : "<<info[i].id
                      <<" run use time : "<<info[i].useTime<<"ms" <<std:: endl;
        }
        std::cout <<"****************************************************************"<<std:: endl;
    }

    ///等待运行完成
    void waitRunFinash(){
        for (auto & thread : threads) {
            if(thread->joinable()){
                thread->join();
            }
//            delete thread;
        }
    }


    bool is_running;  /// 线程池是否还在继续运行
private:

    typedef struct{
        int index;                      //创建
        std::thread::id id;             //线程ID
        float useTime;                  //线程耗时
    }threadInfoType;
    std::vector<threadInfoType> info;

    std::vector<std::thread *> threads; /// 线程容器

    /// 互斥锁 与 条件变量 参见 https://blog.csdn.net/leacock1991/article/details/106820393
    std::mutex m_mutex; /// 互斥锁

    std::condition_variable m_cond; /// 条件变量
    QueueType tasks; /// 队列 排队待处理的任务

    std::map<std::thread::id, clock_t> timeMap;

};

#endif // THREADPOOL_H

//https://blog.csdn.net/fuhanghang/article/details/114818215
