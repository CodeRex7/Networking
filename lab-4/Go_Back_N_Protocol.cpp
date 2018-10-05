#include <iostream>
#include <pthread.h>
#include <unistd.h>
#include <queue>

using namespace std;

queue<int> frames;
int NACK = -1, ok = 0;
int error[35];

void *send(void *args) {
    int numFrames = (*(int *)args);

    for (int i = 0; !ok ;) {
        if (NACK != -1) {
            i = NACK;
            cout << "Sent frame number:\t" << i << "\n";
            frames = queue<int>();
            frames.push(i);
            i++;
            NACK = -1;
        } else if (i < numFrames) {
            cout << "Sent frame number:\t" << i << "\n";
            frames.push(i);
            i++;
        }

        usleep(1000000);
    }

    pthread_exit(NULL);
}

void *recv(void *args) {
    int numFrames = (*(int *)args);
    int processdFrames = 0;

    while (1) {
        if (!frames.empty()) cout << "Received frame number:\t" << frames.front() << "\n";
        processdFrames++;

        if (!frames.empty() and error[frames.front()]) {
            cout << "Erroneous frame number:\t" << frames.front() << "\n";
            processdFrames--;
            NACK = frames.front();
            while (NACK != -1);
            error[frames.front()] = 0;
            continue;
        }

        if (!frames.empty()) frames.pop();

        if (processdFrames == numFrames) break ;

        usleep(1555555);
    }

    ok = 1;

    pthread_exit(NULL);
}

int main() {
    int numFrames;
    cout << "Enter number of frames to send: ";
    cin >> numFrames;

    error[5] = error[8] = error[13] = error[19] = 1;
    
    pthread_t sendThread, recvThread;

    int sendStatus = pthread_create(&sendThread, NULL, send, (void *)&numFrames);
    int recvStatus = pthread_create(&recvThread, NULL, recv, (void *)&numFrames);

    pthread_join(sendThread, NULL);
    pthread_join(recvThread, NULL);
    pthread_exit(NULL);

    return 0;
}