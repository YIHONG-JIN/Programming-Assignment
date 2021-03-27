#include "FibonacciPQ.hpp"
// 相应函数

// Add a new Person in the forest
// 用双链表实现根列表
void FibonacciPQ::newPerson(Person *newroot)
{
    // the Person root Person shouldn't have any sibling Person
    newroot->Parent = NULL;
    newroot->CutMark = false;
    // add the newroot Person in the root set
    Rootlist.push_back(newroot);
    // judge whether there is a requirement to change the min_ptr
    if (Minptr == NULL)
        Minptr = newroot;
    else if (newroot->Key < Minptr->Key)
        Minptr = newroot;
    Rootsize++;
    return;
}

// the function link is used to merge two Persons in one Person
// 返回成为根节点的ptr。
Person *FibonacciPQ::link(Person *a_ptr, Person *b_ptr)
{
    if (a_ptr->Key > b_ptr->Key) // root a_ptr should be the SonPerson of b_ptr
    {
        addSonPerson(a_ptr, b_ptr);
        // delete a_ptr from the root set
        Rootlist.remove(a_ptr);
        return b_ptr;
    }
    else
    {
        // 同理
        addSonPerson(b_ptr, a_ptr);
        Rootlist.remove(b_ptr);
        return a_ptr;
    }
    // error
    return nullptr;
}

// rebalance the forest after one min_ptr has been popped.
void FibonacciPQ::rebalance()
{
    auto vect_ptr = Rootlist.begin();
    // 防止list的remove操作将一开始的rootlist的迭代器给打爆，用while
    while (vect_ptr != Rootlist.end())
    {
        int pos = (*vect_ptr)->Degree;
        if (NULL == Degreebucket[pos])
        {
            Degreebucket[pos] = (*vect_ptr);
            vect_ptr++;
        }
        else if ((*vect_ptr) == Degreebucket[pos])
        {
            vect_ptr++;
        }
        else
        {
            Person *root = link(Degreebucket[pos], (*vect_ptr));
            Degreebucket[(root->Degree - 1)] = NULL;
            vect_ptr = Rootlist.begin();
        }
    }
    // 更新最小值
    for (auto i = Rootlist.begin(); i != Rootlist.end(); i++)
    {
        if (NULL == Minptr)
            Minptr = (*i);
        else if ((*i)->Key < Minptr->Key)
            Minptr = (*i);
    }
    Rootsize = Rootlist.size();
    return;
}

// add one Person as the SonPerson to another Person
// 假设加入的子节点是没有younger的且链表的最后一个子节点也是没有younger
void FibonacciPQ::addSonPerson(Person *sonPerson, Person *parentPerson)
{
    parentPerson->Son.push_back(sonPerson);
    sonPerson->Parent = parentPerson;
    parentPerson->Degree++;
    return;
}

// cut the SonPerson branch from the Person to be a new Person
void FibonacciPQ::cut(Person *sonPerson, Person *parentPerson)
{
    // cut the sonPerson from its parent Person.
    parentPerson->Son.remove(sonPerson);
    parentPerson->Degree--;
    return;
}

Person *FibonacciPQ::popMin()
{
    Person *return_obj = Minptr;
    Degreebucket[Minptr->Degree] = NULL;
    // 一个一个儿子变成新的
    Person *newroot = (*Minptr->Son.begin());
    while (newroot != NULL)
    {
        cut(newroot, Minptr);
        newPerson(newroot);
        newroot = (*Minptr->Son.begin());
    }
    Rootlist.remove(Minptr);
    Minptr = NULL;
    rebalance();
    PQ_length--;
    cout << "the popped person's id is " << return_obj->ID << endl;
    if (PQ_length > 0)
        cout << "now the top priority is " << Minptr->ID << endl;
    return return_obj;
}

bool FibonacciPQ::cascadingCut(Person *parent)
{
    if (parent == NULL)
    {
        return true;
    }
    else
    {
        if (parent->CutMark == false)
        {
            parent->CutMark = true;
        }
        else
        {
            Person *newroot = (*parent->Son.begin());
            while (newroot != NULL)
            {
                cut(newroot, parent);
                newPerson(newroot);
                newroot = (*parent->Son.begin());
            }
            cut(parent, parent->Parent);
            cascadingCut(parent->Parent);
        }
    }
    return true;
}

// operation list
bool FibonacciPQ::inSert(Person *handle)
{
    cout << "add new person " << handle->ID << endl;
    // handle->arrangeStage = treatment;
    // cout << "the person's current Stage is " << treatment;
    newPerson(handle);
    PQ_length++;
    return true;
}

// 更改handle的key值，返回更改的句柄
Person *FibonacciPQ::decreaseKey(Person *changeStatusPerson, string profession_status, string riskStatus_status)
{
    // 造个替身进行假想比较
    Person *origin_person = stand_in(changeStatusPerson);
    cout << "decrease the statues" << endl;
    changeStatusPerson->setProfession(profession_status);
    changeStatusPerson->setRiskStatus(riskStatus_status);
    if ((changeStatusPerson->Parent != NULL) && (origin_person->isLargerThan(*changeStatusPerson)))
    {
        cout << "the priority has been changed" << endl;
        cut(changeStatusPerson, changeStatusPerson->Parent);
        newPerson(changeStatusPerson);
        cascadingCut(changeStatusPerson->Parent);
    }
    if (changeStatusPerson->isLargerThan(*Minptr))
    {
        cout << "the decrease operation has change the minptr" << endl;
        Minptr = changeStatusPerson;
    }
    delete origin_person;
    cout << "successfully increase the priority of the patient " << changeStatusPerson->name << endl;
    return changeStatusPerson;
}

// change_status
Person *FibonacciPQ::changeStatus(Person *changingPatient, string profession_status, string riskStatus_status)
{
    Person *origin_person = stand_in(changingPatient);
    cout << "now change the status " << endl;
    if (origin_person->isLargerThan(*changingPatient))
    {
        changingPatient->setProfession(profession_status);
        changingPatient->setRiskStatus(riskStatus_status);
        if (changingPatient->Parent != nullptr)
        {
            cut(changingPatient, changingPatient->Parent);
            newPerson(changingPatient);
            cascadingCut(changingPatient->Parent);
        }
        freeSon(changingPatient);
        return changingPatient;
    }
    else
    {
        decreaseKey(changingPatient, profession_status, riskStatus_status);
        return changingPatient;
    }
    return nullptr;
}

bool FibonacciPQ::freeSon(Person *parent_node)
{
    Person *first_son = (*parent_node->Son.begin());
    while (first_son != nullptr)
    {
        cut(first_son, first_son->Parent);
        newPerson(first_son);
        first_son = (*parent_node->Son.begin());
    }
    return true;
}
Person *FibonacciPQ::remove(Person *handle)
{
    // 强制类型转换

    decreaseKey(handle, to_string(handle->getProfession() - 1), to_string(handle->getRiskStatus() - 1));
    popMin();
    return handle;
}

bool FibonacciPQ::isEmpty()
{
    if (PQ_length == 0)
        return true;
    else
        return false;
}

int FibonacciPQ::returnLength()
{
    return PQ_length;
}

bool FibonacciPQ::eatPeople(PeopleLocalQueue *local_queue)
{
    int length = local_queue->getLength();
    cout << "there is " << length << "people needed to be eaten" << endl;
    while (length > 0)
    {
        Person *pop_person = local_queue->popFront();
        cout << "the person's id is " << pop_person->ID << endl;
        inSert(pop_person);
        length--;
    }
    return true;
}

Person *FibonacciPQ::stand_in(Person *copy_person_ptr)
{
    // copy the info of input person node and return it
    Person *stand = new Person;
    stand->profession = copy_person_ptr->getProfession();
    stand->profession = copy_person_ptr->getRiskStatus();
    stand->timeStamp.tm_year = copy_person_ptr->timeStamp.tm_year;
    stand->timeStamp.tm_mon = copy_person_ptr->timeStamp.tm_mon;
    stand->timeStamp.tm_min = copy_person_ptr->timeStamp.tm_min;
    stand->timeStamp.tm_sec = copy_person_ptr->timeStamp.tm_sec;
    return stand;
}