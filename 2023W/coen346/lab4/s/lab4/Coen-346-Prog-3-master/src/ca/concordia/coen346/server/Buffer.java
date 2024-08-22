package ca.concordia.coen346.server;
import java.util.concurrent.locks.Condition;
import java.util.concurrent.locks.Lock;
import java.util.concurrent.locks.ReentrantLock;

public class Buffer {
    public final static int BUFFER_SIZE = 10;

    private int[] buffer = new int[BUFFER_SIZE];
    private int in;
    private int out;
    private int count;

    // create Lock and Condition objects for synchronization
    public final Lock lock = new ReentrantLock();
    public final Condition notEmpty = lock.newCondition();
    public final Condition notFull = lock.newCondition();


    public Buffer(){

    }

    public int readCount(){
        lock.lock(); //acquire lock
        try {
            return count;
        }
        finally {
            lock.unlock(); //release lock
        }
    }

    public int getNextItem(int pos){
        lock.lock();
        try {
            int item = buffer[pos];
            return item;
        }
        finally {
            lock.unlock();
        }
    }

    public void setIn(int pos){
        lock.lock();
        try {
            this.in = pos;
        }
        finally {
            lock.unlock();
        }
    }

    public int getNextPosition(){
        lock.lock();
        try {
            return in;
        }
        finally {
            lock.unlock();
        }
    }

    public void insertItem(int item, int pos) {
        lock.lock();
        try {
            buffer[pos] = item;
            count++;
            notEmpty.signal();
        } finally {
            lock.unlock();
        }
    }

    public int removeItem(int pos) {
        lock.lock();
        try {
            int item = buffer[pos];
            buffer[pos] = 0;
            count--;
            notFull.signal();
            return item;
        } finally {
            lock.unlock();
        }
    }

    public void produce(int item) {
        lock.lock();
        try {
            while (count == BUFFER_SIZE) {
                // buffer is full, wait for a consumer to remove items
                notFull.await();
            }
            int pos = getNextPosition();
            insertItem(item, pos);
            setIn((pos + 1) % BUFFER_SIZE);
            count++;
            // notify any waiting consumers that buffer is not empty anymore
            notEmpty.signal();
        } catch (InterruptedException e) {
            e.printStackTrace();
        } finally {
            lock.unlock();
        }
    }

    public void consume() {
        lock.lock();
        try {
            while (count == 0) {
                // buffer is empty, wait for a producer to add items
                notEmpty.await();
            }
            int pos = (getNextPosition() - 1 + BUFFER_SIZE) % BUFFER_SIZE;
            int item = removeItem(pos);
            getNextItem(pos);
            count--;
            // notify any waiting producers that buffer is not full anymore
            notFull.signal();
        } catch (InterruptedException e) {
            e.printStackTrace();
        } finally {
            lock.unlock();
        }
    }

}
