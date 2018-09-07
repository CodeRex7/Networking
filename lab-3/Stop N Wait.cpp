#include <iostream>
#include <pthread.h>
#include <unistd.h>

using namespace std;

int ack[15], frame[15];

void *send(void *args) {
    int id = (*(int *)args);
    while (!ack[id]) {
        cout << "Sending frame: " << id << "\n";
        frame[id] = 1;
        usleep(2000000);
    }
    pthread_exit(NULL);
}

void *receive(void *args) {
    int id = (*(int *)args);
    while (!frame[id]);
    // if (id == 2) usleep(5000000);
    cout << "Acknowledged frame: " << id << "\n";
    ack[id] = 1;
    pthread_exit(NULL);
}

int main() {
    int totFrames = rand() % 5 + 1;
    cout << "Total frames to send: " << totFrames << "\n";
    pthread_t sendThread, receiveThread;
    for (int i = 1; i <= totFrames; i++) {
        int sendOk = pthread_create(&sendThread, NULL, send, (void *)&i);
        int receiveOk = pthread_create(&receiveThread, NULL, receive, (void *)&i);
        pthread_join(sendThread, NULL);
        pthread_join(receiveThread, NULL);
    }
    pthread_exit(NULL);
    return 0;
}