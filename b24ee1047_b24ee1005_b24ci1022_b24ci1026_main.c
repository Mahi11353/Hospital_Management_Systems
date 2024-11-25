#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_PATIENTS 100
#define MAX_DOCTORS 100


typedef struct patient {
    int ID;
    char name[50];
    int age;
    char gender[50];
    long long int contactNumber;
    int isInpatient; // 1 for inpatient, 0 for outpatient
    char admissionDate[15];
    char dischargeDate[15];
    int doctorID;
    float weight; // added for BMI calculation
    float height; // added for BMI calculation
    struct patient *next;
} Patient;

typedef struct doctor {
    int ID;
    char name[50];
    int age;
    char gender[50];
    char specialization[50];
    long long int contactNumber;
    struct doctor *next;
} Doctor;

typedef struct appointment {
    int patientID;
    int doctorID;
    char date[20];
    struct appointment *next;
} Appointment;

// File Pointers for Data Storage
FILE *patientFile;
FILE *doctorFile;
FILE *appointmentFile;

// Global Pointers for Linked Lists
Patient *patients = NULL;
Doctor *doctors = NULL;
Appointment *appointments = NULL;

void registerPatient();
void addDoctor();
void addAppointment();
void searchPatient();
void searchDoctor();
void listPatients();
void listDoctors();
void savePatientToFile(Patient *p);
void saveDoctorToFile(Doctor *d);
void saveAppointmentToFile(Appointment *a);
void loadDataFromFile();
void calculateBMI(); // BMI Calculation function

int main() {
    int choice;
    loadDataFromFile();

    while (1) {
        printf("\n--- Hospital Management System ---\n");
        printf("1. Register Patient\n");
        printf("2. Add Doctor\n");
        printf("3. Add Appointment\n");
        printf("4. Search Patient\n");
        printf("5. Search Doctor\n");
        printf("6. List All Patients\n");
        printf("7. List All Doctors\n");
        printf("8. Calculate BMI\n"); // New option for BMI calculation
        printf("9. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: registerPatient(); break;
            case 2: addDoctor(); break;
            case 3: addAppointment(); break;
            case 4: searchPatient(); break;
            case 5: searchDoctor(); break;
            case 6: listPatients(); break;
            case 7: listDoctors(); break;
            case 8: calculateBMI(); break; // Handle BMI calculation
            case 9: 
                printf("\n---Exit---\n");
                return 0;
            default: printf("Invalid choice! Please try again.\n");
        }
    }
}

void registerPatient() {
    Patient *newPatient = (Patient *)malloc(sizeof(Patient));
    if (!newPatient) {
        printf("Memory allocation failed.\n");
        return;
    }

    newPatient->ID = (patients == NULL) ? 1 : patients->ID + 1;
    printf("Enter patient name: ");
    scanf(" %[^\n]", newPatient->name);
    printf("Enter age: ");
    scanf("%d", &newPatient->age);
    printf("Enter gender: ");
    scanf(" %[^\n]", newPatient->gender);
    printf("Enter contact number: ");
    scanf("%lld", &newPatient->contactNumber);
    printf("Is the patient an inpatient? (1 for Yes, 0 for No): ");
    scanf("%d", &newPatient->isInpatient);

    if (newPatient->isInpatient) {
        printf("Enter admission date (DD/MM/YYYY): ");
        scanf(" %[^\n]", newPatient->admissionDate);
        printf("Enter discharge date (DD/MM/YYYY): ");
        scanf(" %[^\n]", newPatient->dischargeDate);
    } else {
        strcpy(newPatient->admissionDate, "N/A");
        strcpy(newPatient->dischargeDate, "N/A");
    }

    printf("Enter doctor ID assigned to the patient: ");
    scanf("%d", &newPatient->doctorID);

    // Adding weight and height for BMI calculation
    printf("Enter weight (kg): ");
    scanf("%f", &newPatient->weight);
    printf("Enter height (meters): ");
    scanf("%f", &newPatient->height);

    // Calculate BMI
    float bmi = newPatient->weight / (newPatient->height * newPatient->height);
    printf("BMI: %.2f\n", bmi);

    if (bmi < 18.5) {
        printf("Underweight\n");
    } else if (bmi < 24.9) {
        printf("Normal weight\n");
    } else if (bmi < 29.9) {
        printf("Overweight\n");
    } else {
        printf("Obesity\n");
    }

    // Adding to linked list
    newPatient->next = patients;
    patients = newPatient;

    savePatientToFile(newPatient);
    printf("Patient registered successfully! Patient ID: %d\n", newPatient->ID);
}

void addDoctor() {
    Doctor *newDoctor = (Doctor *)malloc(sizeof(Doctor));
    if (!newDoctor) {
        printf("Memory allocation failed.\n");
        return;
    }

    newDoctor->ID = (doctors == NULL) ? 1 : doctors->ID + 1;
    printf("Enter doctor name: ");
    scanf(" %[^\n]", newDoctor->name);
    printf("Enter specialization: ");
    scanf(" %[^\n]", newDoctor->specialization);
    printf("Enter age: ");
    scanf("%d", &newDoctor->age);
    printf("Enter gender: ");
    scanf(" %[^\n]", newDoctor->gender);
    printf("Enter contact number: ");
    scanf("%lld", &newDoctor->contactNumber);

    // Adding to linked list
    newDoctor->next = doctors;
    doctors = newDoctor;

    saveDoctorToFile(newDoctor);
    printf("Doctor added successfully! Doctor ID: %d\n", newDoctor->ID);
}

void addAppointment() {
    Appointment *newAppointment = (Appointment *)malloc(sizeof(Appointment));
    if (!newAppointment) {
        printf("Memory allocation failed.\n");
        return;
    }

    printf("Enter patient ID: ");
    scanf("%d", &newAppointment->patientID);
    printf("Enter doctor ID: ");
    scanf("%d", &newAppointment->doctorID);
    printf("Enter appointment date (DD/MM/YYYY): ");
    scanf(" %[^\n]", newAppointment->date);

    // Adding to linked list
    newAppointment->next = appointments;
    appointments = newAppointment;

    saveAppointmentToFile(newAppointment);
    printf("Appointment added successfully.\n");
}

void searchPatient() {
    int id;
    printf("Enter patient ID to search: ");
    scanf("%d", &id);

    Patient *current = patients;
    while (current) {
        if (current->ID == id) {
            printf("\nPatient ID: %d\nName: %s\nAge: %d\nGender: %s\nContact: %lld\n",
                current->ID, current->name, current->age, current->gender, current->contactNumber);
            printf("Inpatient: %s\nAdmission Date: %s\nDischarge Date: %s\nDoctor ID: %d\n",
                current->isInpatient ? "Yes" : "No", current->admissionDate, current->dischargeDate, current->doctorID);
            return;
        }
        current = current->next;
    }
    printf("Patient not found!\n");
}

void searchDoctor() {
    int id;
    printf("Enter doctor ID to search: ");
    scanf("%d", &id);

    Doctor *current = doctors;
    while (current) {
        if (current->ID == id) {
            printf("\nDoctor ID: %d\nName: %s\nSpecialization: %s\nContact: %lld\n",
                current->ID, current->name, current->specialization, current->contactNumber);
            return;
        }
        current = current->next;
    }
    printf("Doctor not found!\n");
}

void listPatients() {
    printf("\n--- Patient Records ---\n");
    Patient *current = patients;
    while (current) {
        printf("ID: %d, Name: %s, Age: %d, Gender: %s, Inpatient: %s\n",
               current->ID, current->name, current->age, current->gender,
               current->isInpatient ? "Yes" : "No");
        current = current->next;
    }
}

void listDoctors() {
    printf("\n--- Doctor Records ---\n");
    Doctor *current = doctors;
    while (current) {
        printf("ID: %d, Name: %s, Specialization: %s\n", current->ID, current->name, current->specialization);
        current = current->next;
    }
}

void savePatientToFile(Patient *p) {
    patientFile = fopen("patients.txt", "a");
    if (patientFile) {
        fprintf(patientFile, "%d %s %d %s %lld %d %s %s %d %.2f %.2f\n",
                p->ID, p->name, p->age, p->gender, p->contactNumber, p->isInpatient,
                p->admissionDate, p->dischargeDate, p->doctorID, p->weight, p->height);
        fclose(patientFile);
    }
}

void saveDoctorToFile(Doctor *d) {
    doctorFile = fopen("doctors.txt", "a");
    if (doctorFile) {
        fprintf(doctorFile, "%d %s %d %s %s %lld\n", d->ID, d->name, d->age, d->gender, d->specialization, d->contactNumber);
        fclose(doctorFile);
    }
}

void saveAppointmentToFile(Appointment *a) {
    appointmentFile = fopen("appointments.txt", "a");
    if (appointmentFile) {
        fprintf(appointmentFile, "%d %d %s\n", a->patientID, a->doctorID, a->date);
        fclose(appointmentFile);
    }
}

void loadDataFromFile() {
    patientFile = fopen("patients.txt", "r");
    if (patientFile) {
        while (!feof(patientFile)) {
            Patient *newPatient = (Patient *)malloc(sizeof(Patient));
            if (fscanf(patientFile, "%d %s %d %s %lld %d %s %s %d %f %f\n",
                    &newPatient->ID, newPatient->name, &newPatient->age, newPatient->gender,
                    &newPatient->contactNumber, &newPatient->isInpatient,
                    newPatient->admissionDate, newPatient->dischargeDate, &newPatient->doctorID,
                    &newPatient->weight, &newPatient->height) == 11) {
                newPatient->next = patients;
                patients = newPatient;
            }
        }
        fclose(patientFile);
    }

    doctorFile = fopen("doctors.txt", "r");
    if (doctorFile) {
        while (!feof(doctorFile)) {
            Doctor *newDoctor = (Doctor *)malloc(sizeof(Doctor));
            if (fscanf(doctorFile, "%d %s %d %s %s %lld\n",
                    &newDoctor->ID, newDoctor->name, &newDoctor->age, newDoctor->gender,
                    newDoctor->specialization, &newDoctor->contactNumber) == 6) {
                newDoctor->next = doctors;
                doctors = newDoctor;
            }
        }
        fclose(doctorFile);
    }

    appointmentFile = fopen("appointments.txt", "r");
    if (appointmentFile) {
        while (!feof(appointmentFile)) {
            Appointment *newAppointment = (Appointment *)malloc(sizeof(Appointment));
            if (fscanf(appointmentFile, "%d %d %s\n", &newAppointment->patientID, &newAppointment->doctorID, newAppointment->date) == 3) {
                newAppointment->next = appointments;
                appointments = newAppointment;
            }
        }
        fclose(appointmentFile);
    }
}

void calculateBMI() {
    float weight, height, bmi;
    printf("Enter weight (kg): ");
    scanf("%f", &weight);
    printf("Enter height (meters): ");
    scanf("%f", &height);

    bmi = weight / (height * height);
    printf("\nYour BMI is: %.2f\n", bmi);

    if (bmi < 18.5) {
        printf("Underweight\n");
    } else if (bmi < 24.9) {
        printf("Normal weight\n");
    } else if (bmi < 29.9) {
        printf("Overweight\n");
    } else {
        printf("Obesity\n");
    }
}
