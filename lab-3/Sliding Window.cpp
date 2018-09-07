#include <iostream>
#include <pthread.h>
#include <unistd.h>

using namespace std;

int n;
int lSender, rSender;
int lRecver, rRecver;
int frame[20], ack[20];

void *chk(void *args) {
    int id = (*(int *)args);
    while (!ack[id]);
    if (rRecver != n) rRecver++;
    pthread_exit(NULL);
}

void *send(void *args) {
    pthread_t chkAck;
    while (1) {
        if (lSender <= rRecver) {
            cout << "Sending frame: " << lSender << "\n";
            frame[lSender] = 1;
            usleep(2000000);
            int i = lSender;
            pthread_create(&chkAck, NULL, chk, (void *)&i);
            lSender++;
        }
        if (lSender > n) break;
    }
    pthread_join(chkAck, NULL);
    pthread_exit(NULL);
}

void *recv(void *args) {
    while (1) {
        if (lRecver <= rRecver) {
            while (!frame[lRecver]);
            cout << "Acknowledged frame: " << lRecver << "\n";
            ack[lRecver] = 1;
            usleep(3000000);
            lRecver++;
            if (rSender != n) rSender++;
        }
        if (lRecver > n) break;
    }
    pthread_exit(NULL);
}

int main() {
    int totFrames = 16;
    int block = 4;
    lSender = lRecver = 1;
    rSender = rRecver = 4;
    n = totFrames;
    cout << "Total frames to send: " << totFrames << "\n";
    cout << "Window size: " << block << "\n";
    pthread_t sendThread, recvThread;
    int sendOk = pthread_create(&sendThread, NULL, send, NULL);
    int recvOk = pthread_create(&recvThread, NULL, recv, NULL);
    pthread_join(sendThread, NULL);
    pthread_join(recvThread, NULL);
    pthread_exit(NULL);
    return 0;
}