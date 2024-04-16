#include "main.h"
class imp_res : public Restaurant
{
public:
    class Node_queue
    {
    public:
        customer *cus;
        Node_queue *next;
        Node_queue *prev;
        Node_queue(customer *cus, Node_queue *next = nullptr, Node_queue *prev = nullptr)
        {
            this->cus = cus;
            this->next = next;
            this->prev = prev;
        }
        ~Node_queue() {}
    };
    class Queue
    {
    public:
        int size;
        int cnt_jujutsu, cnt_jurei;
        Node_queue *head, *tail;
        Queue() : size(0), cnt_jujutsu(0), cnt_jurei(0), head(nullptr), tail(nullptr) {}
        ~Queue()
        {
            clear();
        }
        void enqueue(customer *cus)
        {
            if (size == MAXSIZE)
            {
                delete cus;
                return;
            }
            Node_queue *newNode = new Node_queue(cus);
            if (head)
            {
                tail->next = newNode;
                newNode->prev = tail;
                tail = newNode;
            }
            else
                head = tail = newNode;
            size++;
            cus->energy > 0 ? cnt_jujutsu++ : cnt_jurei++;
        }
        customer *removeAt(int index)
        {
            if (size == 0)
                return nullptr;
            Node_queue *tmp = head;
            customer *del;
            if (index == 0)
            {
                head = head->next;
                if (head)
                    head->prev = nullptr;
                else
                    head = tail = nullptr;
            }
            else
            {
                while (index--)
                    tmp = tmp->next;
                if (tmp->next)
                {
                    tmp->prev->next = tmp->next;
                    tmp->next->prev = tmp->prev;
                }
                else
                {
                    tail = tail->prev;
                    tail->next = nullptr;
                }
            }
            del = tmp->cus;
            size--;
            tmp->cus->energy > 0 ? cnt_jujutsu-- : cnt_jurei--;
            delete tmp;
            return del;
        }
        customer *remove(string name)
        {
            Node_queue *tmp = head;
            int i = 0;
            for (; i < size; i++)
            {
                if (tmp->cus->name == name)
                    break;
                tmp = tmp->next;
            }
            return removeAt(i);
        }
        void clear()
        {
            for (int i = 0; i < size; i++)
            {
                Node_queue *del = head;
                head = head->next;
                delete del;
            }
            head = tail = nullptr;
            size = 0;
        }
        void Print()
        {
            Node_queue *tmp = head;
            while (tmp)
            {
                tmp->cus->print();
                tmp = tmp->next;
            }
        }
        customer *getAt(Node_queue *start, int index)
        {
            for (int i = 0; i < index; i++)
                start = start->next;
            return start->cus;
        }
    };
    class Node_time
    {
    public:
        customer *cus;
        bool in_table;
        Node_time *next;
        Node_time *prev;
        Node_time(customer *cus, bool in_table, Node_time *next = nullptr, Node_time *prev = nullptr)
        {
            this->cus = cus;
            this->in_table = in_table;
            this->next = next;
            this->prev = prev;
        }
        ~Node_time() {}
    };
    class Time
    {
    public:
        int size;
        Node_time *head, *tail;
        Time() : size(0), head(nullptr), tail(nullptr) {}
        ~Time()
        {
            clear();
        }
        void enqueue(customer *cus, bool in_table)
        {
            if (size == 2 * MAXSIZE)
            {
                delete cus;
                return;
            }
            Node_time *newNode = new Node_time(cus, in_table);
            if (head)
            {
                tail->next = newNode;
                newNode->prev = tail;
                tail = newNode;
            }
            else
                head = tail = newNode;
            size++;
        }
        customer *removeAt(int index)
        {
            if (size == 0)
                return nullptr;
            Node_time *tmp = head;
            customer *del;
            if (index == 0)
            {
                head = head->next;
                if (head)
                    head->prev = nullptr;
                else
                    head = tail = nullptr;
            }
            else
            {
                while (index--)
                    tmp = tmp->next;
                if (tmp->next)
                {
                    tmp->prev->next = tmp->next;
                    tmp->next->prev = tmp->prev;
                }
                else
                {
                    tail = tail->prev;
                    tail->next = nullptr;
                }
            }
            del = tmp->cus;
            size--;
            delete tmp;
            return del;
        }
        customer *remove(string name)
        {
            Node_time *tmp = head;
            int i = 0;
            for (; i < size; i++)
            {
                if (tmp->cus->name == name)
                    break;
                tmp = tmp->next;
            }
            return removeAt(i);
        }
        void clear()
        {
            for (int i = 0; i < size; i++)
            {
                Node_time *del = head;
                head = head->next;
                delete del;
            }
            head = tail = nullptr;
            size = 0;
        }
        void Print()
        {
            Node_time *tmp = head;
            while (tmp)
            {
                tmp->cus->print();
                tmp = tmp->next;
            }
        }
        bool compare(customer *x, customer *y)
        {
            if (abs(x->energy) > abs(y->energy))
                return true;
            if (abs(x->energy) == abs(y->energy))
            {
                Node_time *tmp = head;
                for (int i = 0; i < size; i++)
                {
                    if (tmp->cus->name == y->name)
                        return false;
                    else if (tmp->cus->name == x->name)
                        return true;
                    tmp = tmp->next;
                }
            }
        }
    };

public:
    int count_cus;
    customer *current;
    Queue *queue;
    Time *time;

    imp_res() : count_cus(0), current(nullptr)
    {
        queue = new Queue();
        time = new Time();
    }
    ~imp_res()
    {
        Node_time *tmp = time->head;
        int time_size = time->size;
        for (int i = 0; i < time_size; i++)
        {
            if (tmp->in_table)
            {
                string name_del = tmp->cus->name;
                tmp = tmp->next;
                customer *del = time->remove(name_del);
                if (count_cus >= 2)
                {
                    del->prev->next = del->next;
                    del->next->prev = del->prev;
                    current = (del->energy > 0) ? del->next : del->prev;
                    del->next = del->prev = nullptr;
                }
                else
                    current = nullptr;
                count_cus--;
                delete del;
            }
            else
            {
                string name_del = tmp->cus->name;
                tmp = tmp->next;
                customer *del = time->remove(name_del);
                delete del;
                delete queue->remove(name_del);
            }
        }
        delete current;
        delete time;
        delete queue;
    }

public:
    void RED(string name, int energy)
    {
        // cout << name << " " << energy << endl;
        if (energy == 0)
            return;

        customer *temp_res = current;
        for (int i = 0; i < count_cus; i++)
        {
            if (name == temp_res->name)
                return;
            temp_res = temp_res->next;
        }
        Node_queue *temp_queue = this->queue->head;
        for (int i = 0; i < queue->size; i++)
        {
            if (name == temp_queue->cus->name)
                return;
            temp_queue = temp_queue->next;
        }
        customer *cus = new customer(name, energy, nullptr, nullptr);
        Node_time *temp_time = time->head;
        bool check = false;
        for (int i = 0; i < time->size; i++)
        {
            if (name == temp_time->cus->name)
            {
                delete temp_time->cus;
                temp_time->cus = cus;
                temp_time->in_table = true;
                check = true;
                break;
            }
            temp_time = temp_time->next;
        }
        if (current == nullptr)
        {
            current = cus;
            count_cus++;
            if (!check)
                time->enqueue(cus, true);
            return;
        }
        if (count_cus == MAXSIZE)
        {
            customer *cus1 = new customer(name, energy, nullptr, nullptr);
            queue->enqueue(cus1);
            time->enqueue(cus, false);
            return;
        }
        if (count_cus == 1)
        {
            current->next = current->prev = cus;
            cus->next = cus->prev = current;
        }
        else
        {
            customer *tmp = current;
            int res = cus->energy - tmp->energy;
            if (count_cus >= MAXSIZE / 2 && count_cus < MAXSIZE)
            {
                int RES = -1e6;
                for (int i = 0; i < count_cus; i++)
                {
                    res = cus->energy - tmp->energy;
                    if (abs(res) > RES)
                    {
                        RES = abs(res);
                        current = tmp;
                    }
                    tmp = tmp->next;
                }
                res = cus->energy - current->energy;
            }
            if (res < 0)
            {
                cus->next = current;
                cus->prev = current->prev;
                current->prev->next = cus;
                current->prev = cus;
            }
            else
            {
                cus->next = current->next;
                cus->prev = current;
                current->next->prev = cus;
                current->next = cus;
            }
        }
        current = cus;
        count_cus++;
        if (!check)
            time->enqueue(cus, true);
    }
    void BLUE(int num)
    {
        // cout << "blue " << num << endl;
        if (num == 0)
            return;
        if (num >= count_cus || num >= MAXSIZE)
        {
            num = count_cus;
        }
        for (int i = 0; i < num; i++)
        {
            Node_time *tmp = time->head;
            customer *del = nullptr;
            int time_size = time->size;
            for (int i = 0; i < time_size; i++)
            {
                if (tmp->in_table)
                {
                    string name_del = tmp->cus->name;
                    del = time->remove(name_del);
                    break;
                }
                else
                    tmp = tmp->next;
            }
            if (count_cus >= 2)
            {
                del->prev->next = del->next;
                del->next->prev = del->prev;
                current = (del->energy > 0) ? del->next : del->prev;
                del->next = del->prev = nullptr;
            }
            else
                current = nullptr;
            count_cus--;
            delete del;
        }
        int size_queue = queue->size;
        int count_empty = MAXSIZE - count_cus;
        for (int i = 0; i < count_empty && i < size_queue; i++)
        {
            customer *newCus = queue->removeAt(0);
            RED(newCus->name, newCus->energy);
            delete newCus;
        }
    }
    void PURPLE()
    {
        // cout << "purple" << endl;
        if (queue->size <= 1)
            return;
        int max = -1e6, step = 0;
        Node_queue *tmp = queue->head;
        int index_max = 0;
        for (int i = 0; i < queue->size; i++)
        {
            if (abs(tmp->cus->energy) >= max)
            {
                max = abs(tmp->cus->energy);
                index_max = i;
            }
            tmp = tmp->next;
        }
        for (int i = (index_max + 1) / 2; i > 2; i /= 2)
        {
            Node_queue *sublist_start = queue->head;
            for (int j = 0; j < i; j++)
            {
                int incr = i, n = index_max + 1 - j;
                for (int k = incr; k < n; k += incr)
                {
                    for (int l = k; l >= incr; l -= incr)
                    {
                        customer *x = queue->getAt(sublist_start, l), *y = queue->getAt(sublist_start, l - incr);
                        if (abs(x->energy) > abs(y->energy))
                        {
                            string temp_name = x->name;
                            int temp_energy = x->energy;
                            x->name = y->name;
                            x->energy = y->energy;
                            y->name = temp_name;
                            y->energy = temp_energy;
                            step++;
                        }
                        else if (abs(x->energy) == abs(y->energy))
                        {
                            if (time->compare(x, y))
                            {
                                string temp_name = x->name;
                                int temp_energy = x->energy;
                                x->name = y->name;
                                x->energy = y->energy;
                                y->name = temp_name;
                                y->energy = temp_energy;
                                step++;
                            }
                        }
                    }
                }
                sublist_start = sublist_start->next;
            }
        }
        for (int k = 1; k < index_max + 1; k += 1)
        {
            for (int j = k; j >= 1; j -= 1)
            {
                customer *x = queue->getAt(queue->head, j), *y = queue->getAt(queue->head, j - 1);
                if (abs(x->energy) > abs(y->energy))
                {
                    string temp_name = x->name;
                    int temp_energy = x->energy;
                    x->name = y->name;
                    x->energy = y->energy;
                    y->name = temp_name;
                    y->energy = temp_energy;
                    step++;
                }
                else if (abs(x->energy) == abs(y->energy))
                {
                    if (time->compare(x, y))
                    {
                        string temp_name = x->name;
                        int temp_energy = x->energy;
                        x->name = y->name;
                        x->energy = y->energy;
                        y->name = temp_name;
                        y->energy = temp_energy;
                        step++;
                    }
                }
            }
        }
        BLUE(step % MAXSIZE);
    }
    void REVERSAL()
    {
        // cout << "reversal" << endl;
        if (count_cus == 2)
            return;
        Queue *jujutsu = new Queue();
        Queue *jurei = new Queue();
        customer *tmp = current;
        for (int i = 0; i < count_cus; i++)
        {
            if (tmp->energy > 0)
                jujutsu->enqueue(tmp);
            else
                jurei->enqueue(tmp);
            tmp = tmp->prev;
        }
        Node_queue *headNode = jujutsu->head, *tailNode = jujutsu->tail;
        customer *head, *tail;
        for (int i = 0; i < jujutsu->size / 2; i++)
        {
            head = headNode->cus;
            tail = tailNode->cus;
            if (head->prev != tail && head->next != tail)
            {
                head->next->prev = tail;
                tail->prev->next = head;
                head->prev->next = tail;
                tail->next->prev = head;
                customer *tmp = new customer("", 0, head->prev, head->next);
                head->next = tail->next;
                head->prev = tail->prev;
                tail->next = tmp->next;
                tail->prev = tmp->prev;
                tmp->next = tmp->prev = nullptr;
                delete tmp;
            }
            else
            {
                if (head->prev == tail)
                {
                    head->prev = tail->prev;
                    tail->next = head->next;
                    head->next->prev = tail;
                    tail->prev->next = head;
                    head->next = tail;
                    tail->prev = head;
                }
                else
                {
                    head->next = tail->next;
                    tail->prev = head->prev;
                    head->prev->next = tail;
                    tail->next->prev = head;
                    head->prev = tail;
                    tail->next = head;
                }
            }
            headNode = headNode->next;
            tailNode = tailNode->prev;
        }
        delete jujutsu;
        headNode = jurei->head;
        tailNode = jurei->tail;
        for (int i = 0; i < jurei->size / 2; i++)
        {
            head = headNode->cus;
            tail = tailNode->cus;
            if (head->prev != tail && head->next != tail)
            {
                head->next->prev = tail;
                tail->prev->next = head;
                head->prev->next = tail;
                tail->next->prev = head;
                customer *tmp = new customer("", 0, head->prev, head->next);
                head->next = tail->next;
                head->prev = tail->prev;
                tail->next = tmp->next;
                tail->prev = tmp->prev;
                tmp->next = tmp->prev = nullptr;
                delete tmp;
            }
            else
            {
                if (head->prev == tail)
                {
                    head->prev = tail->prev;
                    tail->next = head->next;
                    head->next->prev = tail;
                    tail->prev->next = head;
                    head->next = tail;
                    tail->prev = head;
                }
                else
                {
                    head->next = tail->next;
                    tail->prev = head->prev;
                    head->prev->next = tail;
                    tail->next->prev = head;
                    head->prev = tail;
                    tail->next = head;
                }
            }
            headNode = headNode->next;
            tailNode = tailNode->prev;
        }
        delete jurei;
    }
    void UNLIMITED_VOID()
    {
        // cout << "unlimited_void" << endl;
        if (count_cus < 4)
            return;
        int min = 1e6;
        customer *flag = nullptr;
        customer *start = current;
        int length = 4;
        for (int i = 0; i < count_cus; i++)
        {
            customer *tmp = start;
            int sum = 0;
            for (int j = 0; j < 3; j++)
            {
                sum += tmp->energy;
                tmp = tmp->next;
            }
            for (int j = 3; j < count_cus; j++)
            {
                sum += tmp->energy;
                if (sum < min)
                {
                    flag = start;
                    length = j + 1;
                    min = sum;
                }
                else if (sum == min && j + 1 >= length)
                {
                    flag = start;
                    length = j + 1;
                    min = sum;
                }
                tmp = tmp->next;
            }
            start = start->next;
        }

        int index_min = 0;
        min = 1e6;
        customer *tmp = flag, *cus_min = nullptr;
        for (int i = 0; i < length; i++)
        {
            if (tmp->energy < min) // xoa dau = cho ni
            {
                cus_min = tmp;
                index_min = i;
                min = tmp->energy;
            }
            tmp = tmp->next;
        }
        for (int i = 0; i < length - index_min; i++)
        {
            cus_min->print();
            cus_min = cus_min->next;
        }
        for (int i = 0; i < index_min; i++)
        {
            flag->print();
            flag = flag->next;
        }
    }
    void DOMAIN_EXPANSION()
    {
        // cout << "domain_expansion" << endl;
        long long jujutsu_energy = 0, jurei_energy = 0;
        Node_time *temp = time->head;
        for (int i = 0; i < time->size; i++)
        {
            if (temp->cus->energy > 0)
            {
                jujutsu_energy += temp->cus->energy;
            }
            else
                jurei_energy += temp->cus->energy;
            temp = temp->next;
        }
        temp = time->tail;
        for (int i = 0; i < time->size; i++)
        {
            if (jujutsu_energy >= abs(jurei_energy) && temp->cus->energy < 0) // jurei lose
                temp->cus->print();
            else if (jujutsu_energy < abs(jurei_energy) && temp->cus->energy > 0) // jujutsu lose
                temp->cus->print();
            temp = temp->prev;
        }
        temp = time->head;
        int time_size = time->size;
        for (int i = 0; i < time_size; i++)
        {
            customer *del;
            string name_del = temp->cus->name;
            if (jujutsu_energy >= abs(jurei_energy) && temp->cus->energy < 0 && temp->in_table)
            {
                temp = temp->next;
                del = time->remove(name_del);
            }
            else if (jujutsu_energy < abs(jurei_energy) && temp->cus->energy > 0 && temp->in_table)
            {
                temp = temp->next;
                del = time->remove(name_del);
            }
            else
            {
                temp = temp->next;
                continue;
            }
            if (count_cus >= 2)
            {
                del->prev->next = del->next;
                del->next->prev = del->prev;
                current = (del->energy > 0) ? del->next : del->prev;
                del->next = del->prev = nullptr;
            }
            else
                current = nullptr;
            count_cus--;
            delete del;
        }
        Node_queue *tmp = queue->head;
        int size_queue = queue->size;
        for (int i = 0; i < size_queue; i++)
        {
            string name_del = tmp->cus->name;
            if (jujutsu_energy >= abs(jurei_energy) && tmp->cus->energy < 0)
            {
                tmp = tmp->next;
                delete queue->remove(name_del);
                delete time->remove(name_del);
            }
            else if (jujutsu_energy < abs(jurei_energy) && tmp->cus->energy > 0)
            {
                tmp = tmp->next;
                delete queue->remove(name_del);
                delete time->remove(name_del);
            }
            else
                tmp = tmp->next;
        }
        size_queue = queue->size;
        int count_empty = MAXSIZE - count_cus;
        for (int i = 0; i < size_queue && i < count_empty; i++)
        {
            customer *newCus = queue->removeAt(0);
            RED(newCus->name, newCus->energy);
            delete newCus;
        }
    }
    void LIGHT(int num)
    {
        // cout << "light " << num << endl;
        if (num == 0)
            queue->Print();
        else
        {
            customer *tmp = current;
            for (int i = 0; i < count_cus; i++)
            {
                tmp->print();
                tmp = (num > 0) ? tmp->next : tmp->prev;
            }
        }
    }
};