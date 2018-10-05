#include <iostream>
#include <pthread.h>
#include <unistd.h>
#include <vector>
#include <algorithm>

using namespace std;

vector<int> frames;
int NACK = -1, ok = 0;
int error[35];

void *send(void *args) {
    int numFrames = (*(int *)args);

    for (int i = 0; !ok; ) {
        if (NACK != -1) {
            cout << "Resent frame number:\t" << NACK << "\n";
            frames.push_back(NACK);
            NACK = -1;
        } else if (i < numFrames) {
            cout << "Sent frame number:\t" << i << "\n";
            frames.push_back(i);
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
        if (!frames.empty()) {
            cout << "Received frame number:\t" << frames[0] << "\n";
            processdFrames++;
        }

        if (!frames.empty() and error[frames[0]]) {
            cout << "Erroneous frame number:\t" << frames[0] << "\n";
            processdFrames--;
            NACK = frames[0];
            frames.erase(frames.begin());
            while (NACK != -1);
            sort(frames.begin(), frames.end());
            error[frames[0]] = 0;
            continue ;
        }

        if (!frames.empty()) {
            frames.erase(frames.begin());
        }
        
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
    
    int n = rand() % min(5, numFrames);
    for (int i = 0; i < n; i++) {
        int x = rand() % numFrames;
        error[x] = 1;
    }

    pthread_t sendThread, recvThread;

    int sendStatus = pthread_create(&sendThread, NULL, send, (void *)&numFrames);
    int recvStatus = pthread_create(&recvThread, NULL, recv, (void *)&numFrames);

    pthread_join(sendThread, NULL);
    pthread_join(recvThread, NULL);
    pthread_exit(NULL);

    return 0;
}