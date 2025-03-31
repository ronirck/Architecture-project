#include "./headers.hpp"
#ifndef PARTEMEDICA_H
#define PARTEMEDICA_H



using json = nlohmann::json;
using string = std::string;
using std::vector;

struct Patient {
    int id;
    string name;
    string diagnosis;
    string admission_time;
    string sex;
    int age;
    int height;
    vector<string> medications;
};
void clearScreenAfterInput();
void diseasesByAge(const vector<Patient>& patients);
void medicationsUsage(const vector<Patient>& patients);
void diseasesBySex(const vector<Patient>& patients);
void admissionsByYear(const vector<Patient>& patients);
vector<Patient> getPatients(int patientsPerPage, int currentPage);
int selectPatient(int &currentPage, const int patientsPerPage);
void showPatientDetails(const Patient &patient);
vector<Patient> getAllPatients();


#endif