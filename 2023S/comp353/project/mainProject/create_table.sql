CREATE TABLE Person (
    MedicareCardNumber INT PRIMARY KEY NOT NULL,
    FirstName VARCHAR(50) NOT NULL,
    LastName VARCHAR(50) NOT NULL,
    DateOfBirth DATE NOT NULL,
    Address VARCHAR(100) NOT NULL,
    City VARCHAR(50) NOT NULL,
    Province VARCHAR(50) NOT NULL,
    PostalCode VARCHAR(10) NOT NULL,
    Citizenship VARCHAR(50) NOT NULL,
    EmailAddress VARCHAR(100) NOT NULL,
    TelephoneNumber VARCHAR(20) NOT NULL,
    MedicareExpirationDate DATE NOT NULL
);

CREATE TABLE Ministry (
    MinistryID INT PRIMARY KEY NOT NULL,
    HeadOfficeFacilityAddress VARCHAR(100) NOT NULL,
    MinisterOfEducation INT NOT NULL,
    FOREIGN KEY (MinisterOfEducation) REFERENCES Person(MedicareCardNumber)
);

CREATE TABLE Facility (
    FacilityID INT NOT NULL,
    MinistryID INT NOT NULL,
    Name VARCHAR(100) NOT NULL,
    Address VARCHAR(100) NOT NULL,
    City VARCHAR(50) NOT NULL,
    Province VARCHAR(50) NOT NULL,
    CapOnEmployees INT NOT NULL,
    PostalCode VARCHAR(10) NOT NULL,
    Phone VARCHAR(20) NOT NULL,
    WebAddress VARCHAR(100) NOT NULL,
    Type VARCHAR(50) NOT NULL,
    FOREIGN KEY (MinistryID) REFERENCES Ministry(MinistryID),
    PRIMARY KEY (FacilityID, MinistryID)
);

CREATE TABLE Educational (
    FacilityID INT NOT NULL ,
    MinistryID INT NOT NULL,
    Principle INT NOT NULL,
    Type VARCHAR(50) NOT NULL,
    FOREIGN KEY (FacilityID) REFERENCES Facility(FacilityID),
    FOREIGN KEY (MinistryID) REFERENCES Ministry(MinistryID),
    FOREIGN KEY (Principle) REFERENCES Person(MedicareCardNumber),
    PRIMARY KEY (FacilityID, MinistryID)
);

CREATE TABLE Infection (
    DateOfInfection DATE NOT NULL,
    MedicareCardNumber INT NOT NULL,
    InfectionType VARCHAR(50) NOT NULL,
    FOREIGN KEY (MedicareCardNumber) REFERENCES Person(MedicareCardNumber),
    PRIMARY KEY (MedicareCardNumber, DateOfInfection)
);

CREATE TABLE Vaccination (
    DoseNumber INT NOT NULL,
    Date DATE NOT NULL,
    Type VARCHAR(50) NOT NULL,
    MedicareCardNumber INT NOT NULL,
    FOREIGN KEY (MedicareCardNumber) REFERENCES Person(MedicareCardNumber),
    PRIMARY KEY (DoseNumber, MedicareCardNumber)
);

CREATE TABLE Student (
    MedicareCardNumber INT PRIMARY KEY NOT NULL,
    FacilityID INT NOT NULL,
    MinistryID INT NOT NULL,
    Level VARCHAR(50) NOT NULL,
    FOREIGN KEY (MedicareCardNumber) REFERENCES Person(MedicareCardNumber),
    FOREIGN KEY (FacilityID) REFERENCES Facility(FacilityID),
    FOREIGN KEY (MinistryID) REFERENCES Ministry(MinistryID)	
);

CREATE TABLE Employee (
    Role VARCHAR(50) NOT NULL,
    MedicareCardNumber INT PRIMARY KEY NOT NULL,
    FacilityID INT NOT NULL,
    MinistryID INT NOT NULL,
    FOREIGN KEY (MedicareCardNumber) REFERENCES Person(MedicareCardNumber),
    FOREIGN KEY (FacilityID) REFERENCES Facility(FacilityID),
    FOREIGN KEY (MinistryID) REFERENCES Ministry(MinistryID)
);

CREATE TABLE Management (
    FacilityID INT NOT NULL,
    MinistryID INT NOT NULL,
    PresidentID INT NOT NULL,
    HeadOffice BOOLEAN NOT NULL,
    PRIMARY KEY (FacilityID, MinistryID),
    FOREIGN KEY (FacilityID, MinistryID) REFERENCES Facility(FacilityID, MinistryID),
    FOREIGN KEY (PresidentID) REFERENCES Person(MedicareCardNumber)
);

CREATE TABLE SchoolsAttendedByStudents (
    MedicareCardNumber INT NOT NULL,
    FacilityID INT NOT NULL,
    MinistryID INT NOT NULL,
    Level VARCHAR(50) NOT NULL,
    StartDate DATE NOT NULL,
    EndDate DATE,
    PRIMARY KEY (MedicareCardNumber, FacilityID,MinistryID),
    FOREIGN KEY (MedicareCardNumber) REFERENCES Person(MedicareCardNumber),
    FOREIGN KEY (FacilityID, MinistryID) REFERENCES Facility(FacilityID, MinistryID)
);

CREATE TABLE WorkplaceForEachEmployee (
    Role VARCHAR(50) NOT NULL,
    MedicareCardNumber INT NOT NULL,
    FacilityID INT NOT NULL,
    MinistryID INT NOT NULL,
    StartDate DATE NOT NULL,
    EndDate DATE,
    PRIMARY KEY (MedicareCardNumber,FacilityID, MinistryID),
    FOREIGN KEY (MedicareCardNumber) REFERENCES Person(MedicareCardNumber),
    FOREIGN KEY (FacilityID, MinistryID) REFERENCES Facility(FacilityID, MinistryID)
);

CREATE TABLE EmployeeSchedule (
    FacilityID INT NOT NULL,
    MinistryID INT NOT NULL,
    MedicareCardNumber INT NOT NULL,
    Date DATE NOT NULL,
    StartTime TIME NOT NULL,
    EndTime TIME NOT NULL,
    PRIMARY KEY (FacilityID, MinistryID, MedicareCardNumber, Date),
    FOREIGN KEY (FacilityID, MinistryID) REFERENCES Facility(FacilityID, MinistryID),
    FOREIGN KEY (MedicareCardNumber) REFERENCES Person(MedicareCardNumber)
);
