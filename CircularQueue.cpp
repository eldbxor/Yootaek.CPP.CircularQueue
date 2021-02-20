#include "CircularQueue.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

CircularQueue*
CircularQueue::createNew() {
    CircularQueue* newSource = new CircularQueue();
    return newSource;
}

CircularQueue::CircularQueue() {
    pthread_mutex_init(&mutex_lock, NULL);
}

CircularQueue::~CircularQueue() {

}

int CircularQueue::put(unsigned char* buf, int bufLen) {
    pthread_mutex_lock(&mutex_lock);

    if (rear < front && ((rear + bufLen + 1) >= front)) {
        fprintf(stderr, "Queue Overflow.\n"); 
        return -1;
    }

    int bufLenFromZero = rear + bufLen - MAXSIZE;
    if (bufLenFromZero > 0) {
        int firstWriteSize = MAXSIZE - rear;
        memcpy(queue + rear + 1, buf, firstWriteSize);
        memcpy(queue, buf + firstWriteSize, bufLenFromZero);
        rear = bufLenFromZero;
    }
    else {
        memcpy(queue + rear, buf, bufLen);
        rear += bufLen;
    }
    
    // fprintf(stderr, "[CircularQueue] front: %d, rear: %d\n", front, rear);
    pthread_mutex_unlock(&mutex_lock);
    return 0;
}

int CircularQueue::get(unsigned char* buf, int bufLen) {
    pthread_mutex_lock(&mutex_lock);
    if (front == rear) {
        fprintf(stderr, "Queue Underflow.\n");
        return -1;
    }

    int bufLenFromZero = (front + bufLen) - MAXSIZE;
    if (bufLenFromZero > 0) {
        int firstReadSize = MAXSIZE - front;
        memcpy(buf, queue + front, firstReadSize);
        if (bufLenFromZero > rear) {
            fprintf(stderr, "Queue Underflow.\n");
            return -1;
        }
        memcpy(buf, queue, bufLenFromZero);
        front = bufLenFromZero + 1;
    }
    else {
        if ((rear > front) && ((front + bufLen) >= rear)) {
            fprintf(stderr, "Queue Underflow.\n");
            return -1;
        }
        memcpy(buf, queue + front, bufLen);
        front += bufLen;
    }

    // fprintf(stderr, "[CircularQueue] front: %d, rear: %d\n", front, rear);
    pthread_mutex_unlock(&mutex_lock);
    return 0;
}