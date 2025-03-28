#include <iostream>
#include <fstream>
#include <random>
#include <string>
#include <vector>
#include <lmdb.h>
#include "json.hpp"  // Incluye la biblioteca de JSON

using json = nlohmann::json;

// Función para generar un nombre y apellido aleatorio
std::string generateRandomName() {
    std::vector<std::string> firstNames = {"Juan", "María", "Carlos", "Ana", "Luis", "Laura", "Pedro", "Sofía", "Miguel", "Elena"};
    std::vector<std::string> lastNames = {"González", "Rodríguez", "Fernández", "López", "Díaz", "Martínez", "Pérez", "Sánchez", "Ramírez", "Torres"};
    return firstNames[rand() % firstNames.size()] + " " + lastNames[rand() % lastNames.size()];
}

// Función para generar una fecha aleatoria en formato YYYY-MM-DD
std::string generateRandomDate() {
    int year = 2025;
    int month = rand() % 12 + 1;
    int day = rand() % 28 + 1;

    return std::to_string(year) + "-" +
           (month < 10 ? "0" : "") + std::to_string(month) + "-" +
           (day < 10 ? "0" : "") + std::to_string(day);
}

// Función para generar un diagnóstico aleatorio
std::string generateRandomDiagnosis() {
    std::vector<std::string> diagnoses = {
        "Sepsis", "Pneumonia", "Heart failure", "Asthma", "Diabetes", "Stroke", "Chronic obstructive pulmonary disease", "Acute kidney injury", "Cancer", 
        "Hypertension", "Alzheimer's disease", "Parkinson's disease", "Tuberculosis", "Osteoarthritis", "Rheumatoid arthritis", "Chronic heart disease", 
        "Liver cirrhosis", "Chronic renal disease", "Epilepsy", "Multiple sclerosis", "Anemia", "Hypothyroidism", "Hyperthyroidism", "Obesity", 
        "Gastroesophageal reflux disease", "Lung cancer", "Cervical cancer", "Prostate cancer", "Breast cancer", "Leukemia", "Sickle cell anemia", 
        "Cystic fibrosis", "AIDS", "HIV", "Chronic migraine", "Asthma attack", "Tuberculous meningitis", "Bipolar disorder", "Schizophrenia", "Cystitis", 
        "Bronchitis", "Hepatitis B", "Hepatitis C", "Polycystic ovary syndrome", "Sleep apnea", "Depression", "Post-traumatic stress disorder", 
        "Peptic ulcer disease", "Irritable bowel syndrome", "Appendicitis", "Gallstones", "Kidney stones", "Urinary tract infection", "Gout", "Psoriasis", 
        "Eczema", "Lupus", "Fibromyalgia", "Migraine", "Hernia", "Pancreatitis", "HIV/AIDS", "Gastritis", "Influenza", "Sinusitis", "Pneumothorax", "Meningitis"
    };
    return diagnoses[rand() % diagnoses.size()];
}

// Función para generar un sexo aleatorio
std::string generateRandomSex() {
    return rand() % 2 == 0 ? "Male" : "Female";
}
int generateRandomWeight() { return rand() % 100 + 50; }
// Función para generar una altura aleatoria (en cm)
int generateRandomHeight() {
    return rand() % 50 + 150;
}


// Función para generar una lista de medicamentos aleatorios
std::vector<std::string> generateRandomMedications() {
    std::vector<std::string> medications = {
        "Aspirin", "Ibuprofen", "Paracetamol", "Insulin", "Amoxicillin", "Penicillin", "Prednisone", "Lisinopril", "Atorvastatin", "Atenolol",
        "Metformin", "Furosemide", "Warfarin", "Losartan", "Amlodipine", "Clopidogrel", "Zolpidem", "Omeprazole", "Simvastatin", "Hydrochlorothiazide",
        "Gabapentin", "Citalopram", "Diazepam", "Lorazepam", "Azithromycin", "Levofloxacin", "Albuterol", "Fluticasone", "Budesonide", "Sildenafil",
        "Tamsulosin", "Ranitidine", "Montelukast", "Ciprofloxacin", "Ceftriaxone", "Doxycycline", "Cephalexin", "Corticosteroids", "Meloxicam", "Allopurinol",
        "Tramadol", "Methotrexate", "Chloroquine", "Diphenhydramine", "Hydrocodone", "Oxycodone", "Tizanidine", "Carvedilol", "Ezetimibe", "Propranolol",
        "Loratadine", "Trazodone", "Baclofen", "Clonazepam", "Methylprednisolone", "Hydrocortisone", "Sertraline", "Mirtazapine", "Fluoxetine", "Venlafaxine"
    };

    int numMedications = rand() % 3 + 1;
    std::vector<std::string> patientMedications;
    
    for (int i = 0; i < numMedications; ++i) {
        patientMedications.push_back(medications[rand() % medications.size()]);
    }

    return patientMedications;
}

int main() {
    srand(time(0));
    MDB_env* env;
    MDB_dbi dbi;
    MDB_txn* txn;
    MDB_val key, data;

    mdb_env_create(&env);
    
    // Reservar espacio de 3 GB para la base de datos
    mdb_env_set_mapsize(env, 3L * 1024 * 1024 * 1024);  // 3 GB
    
    mdb_env_open(env, "./base_de_datos_lmdb", 0, 0664);
    mdb_txn_begin(env, nullptr, 0, &txn);
    mdb_dbi_open(txn, nullptr, MDB_CREATE, &dbi);

    int numPatients = 58800000;
    for (int i = 0; i < numPatients; ++i) {
        json patient;
        int patient_id = 100 + rand() % 1000;
        patient["patient_id"] = patient_id;
        patient["name"] = generateRandomName(); // Agrega esta línea para asignar un nombre aleatorio
        patient["age"] = 18 + rand() % 80;
        patient["diagnosis"] = generateRandomDiagnosis();
        patient["admission_time"] = generateRandomDate();
        patient["sex"] = generateRandomSex();
        patient["weight"] = generateRandomWeight();
        patient["height"] = generateRandomHeight();
        patient["medications"] = generateRandomMedications();

        std::string patientData = patient.dump();
        key.mv_size = sizeof(patient_id);
        key.mv_data = &patient_id;
        data.mv_size = patientData.size();
        data.mv_data = (void*)patientData.c_str();
        mdb_put(txn, dbi, &key, &data, 0);
    }

    mdb_txn_commit(txn);  // Usamos commit en lugar de abort para guardar los cambios
    mdb_dbi_close(env, dbi);
    mdb_env_close(env);

    return 0;
}
















