CREATE TABLE person(
    personId INT NOT NULL AUTO_INCREMENT,
    personfirstName VARCHAR(255) NOT NULL,
    personlastName VARCHAR(255) NOT NULL,
    personDateOfBirth DATE NOT NULL,
	personMedicareCardNumber VARCHAR(255) NOT NULL,
	personMedicareExpiryDate DATE NOT NULL,
	personPhoneNumber VARCHAR(255) NOT NULL,
    personAddress VARCHAR(255) NOT NULL,
    personCity VARCHAR(255) NOT NULL,
	personProvince ENUM('Alberta', 'British Columbia', 'Manitoba', 'New Brunswick', 'Newfoundland and Labrador', 'Nova Scotia', 'Ontario', 'Prince Edward Island', 'Quebec', 'Saskatchewan') NOT NULL,
    personPostalCode VARCHAR(255) NOT NULL,
	personCitizenship ENUM('citizen by birth', 'citizen by naturalization', 'non-citizen') NOT NULL,
	personEmailAddress VARCHAR(255) NOT NULL,
	personRole ENUM('student', 'employee') NOT NULL,
    PRIMARY KEY(personId)
);

CREATE TABLE facility (
    facilityId INT NOT NULL AUTO_INCREMENT,
    facilityName VARCHAR(255) NOT NULL,
    facilityAddress VARCHAR(255) NOT NULL,
    facilityCity VARCHAR(255) NOT NULL,
    facilityProvince ENUM('Alberta', 'British Columbia', 'Manitoba', 'New Brunswick', 'Newfoundland and Labrador', 'Nova Scotia', 'Ontario', 'Prince Edward Island', 'Quebec', 'Saskatchewan') NOT NULL,
    facilityPostalCode VARCHAR(255) NOT NULL,
    facilityPhoneNumber VARCHAR(255) NOT NULL,
    facilityWebAddress VARCHAR(255) NOT NULL,
    capOnEmployees INT NOT NULL,
    facilityType ENUM('management', 'educational') NOT NULL,
	facilityPresident VARCHAR(255) NOT NULL,
	personID INT NOT NULL,
    PRIMARY KEY (facilityId),
	FOREIGN KEY (personId) REFERENCES person(personId)
); 

CREATE TABLE infection (
	infectionId INT NOT NULL AUTO_INCREMENT,
	infectionDate DATE NOT NULL,
	infectionType ENUM('alpha', 'beta', 'gamma', 'delta', 'omicron') NOT NULL,
	personId INT NOT NULL,
	PRIMARY KEY(infectionId),
	FOREIGN KEY (personId) REFERENCES person(personId)
);

CREATE TABLE vaccination (
    vaccinationID INT NOT NULL AUTO_INCREMENT,
    vaccinationDate DATE NOT NULL,
    doseNumber INT NOT NULL,
    vaccinationType ENUM('Pfizer', 'Moderna', 'AstraZeneca', 'Johnson & Johnson') NOT NULL,
    personId INT NOT NULL,
    PRIMARY KEY (vaccinationID),
    FOREIGN KEY (personId) REFERENCES person(personId)
);

CREATE TABLE ministry (
	ministryId INT NOT NULL AUTO_INCREMENT,
	ministryHeadName VARCHAR(255) NOT NULL,
	ministryFacilityId INT NOT NULL,
	PRIMARY KEY (ministryId),
	FOREIGN KEY (ministryFacilityId) REFERENCES facility(facilityId)
);



