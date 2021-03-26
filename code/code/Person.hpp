#ifndef database_h
#define database_h
#include <math.h>
#include <stdio.h>

#include <cstring>
#include <ctime>
#include <iostream>
#include <list>
#include <vector>

using namespace std;

enum personStage{buffer, treatment, appointment};
class Person {
 public:
  friend class FibonacciPQ;
  friend class blackList;

  string getName(void) { return name; }
  string getID(void) { return ID; }
  string getContactDetails(void) { return contactDetails; }
  tm getBirthdate(void) { return birthDate; }
  tm getTimestamp(void) { return timeStamp; }
  int getProfession(void) { return profession; }
  int getRiskStatus(void) { return riskStatus; }
  int getLocation(void) { return location; }

  void setID(char *data);
  void setName(char *data);
  void setContactDetails(char *data);
  void setBirthYear(char *data);
  void setBirthMonth(char *data);
  void setBirthDay(char *data);
  void setTimestamp(char *data);
  void setProfession(char *data);
  void setRiskStatus(char *data);
  void setLocation(char *data);

  // Jiahao
  bool assignTime(void);
  // liyang
  bool withdraw(vector<Person> *blacklist_ptr);
  // baihao
  bool init();
  // yihong
  bool isLargerThan(Person person);

 private:
  string ID;
  string name;
  string contactDetails;
  tm birthDate;
  tm timeStamp;
  int profession, riskStatus;
  int location;
  int ageGroup;
  personStage arrangeStage;
  // Jiahao
  tm appointmentTime;
  // added by Liyang in 3.19
  bool Store = true;
  int Degree = 0;
  bool CutMark = false;
  list<Person *> Son = list<Person *>();
  Person *Parent = nullptr;
  // added by liyang in 3.25
  int Key = 0;
};


#endif
