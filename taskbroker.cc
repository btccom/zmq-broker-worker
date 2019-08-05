#include "zmq.hpp"

#include <stdlib.h> 
#include <iostream>
#include <chrono>
#include <thread>
using namespace std;

void publishtask(zmq::context_t *context) {
    zmq::socket_t sender(*context, ZMQ_PUSH);
    sender.bind("tcp://*:6666");
    for (int task_nbr = 0; task_nbr < 10000; task_nbr++) {
        char task [10] = {0};
        sprintf (task, "%d", task_nbr);
        std::cout << task << std::endl;
        std::string taskstr(task);
        sender.send(taskstr.c_str(), taskstr.length());
        std::this_thread::sleep_for(std::chrono::milliseconds(100)); 
    }
    sender.close();
}

void resultcollect(zmq::context_t *context) {
    zmq::socket_t sink(*context, ZMQ_PULL);
    sink.bind("tcp://*:5558");
    zmq::message_t zcontent;
    while(1) {
        try {
            sink.recv(&zcontent);
        } catch (zmq::error_t &e) {
            std::cout << " zmq recv exception: " << e.what();
            break;
        }
        const string content = std::string(static_cast<char *>(zcontent.data()), zcontent.size());
        cout << "type : " << content << endl;
    }
    sink.close();
}

int main(int argc, char* arggv[]) {
    zmq::context_t *context = new zmq::context_t(1);
    std::thread pushthread = thread(publishtask, context);
    std::thread pullthread = thread(resultcollect, context);

    pushthread.join();
    pullthread.join();

    return 0;
}