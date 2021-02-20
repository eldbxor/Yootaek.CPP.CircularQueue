#include <pthread.h>

#define MAXSIZE 64 * 1024

class CircularQueue {
public:
  unsigned int getFront() { return front; }
  unsigned int getRear() { return rear; }
  unsigned int getMaxSize() { return MAXSIZE; }
  int put(unsigned char* buf, int bufLen);
  int get(unsigned char* buf, int bufLen);

  static CircularQueue* createNew();

protected:
  CircularQueue();
	// called only by createNew()

  virtual ~CircularQueue();

private:
    unsigned int front = 0, rear = 0;
    unsigned char queue[MAXSIZE];
    pthread_mutex_t mutex_lock;
};