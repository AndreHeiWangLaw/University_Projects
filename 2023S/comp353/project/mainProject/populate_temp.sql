-- Person table
INSERT INTO Person (MedicareCardNumber, FirstName, LastName, DateOfBirth, Address, City, Province, PostalCode, Citizenship, EmailAddress, TelephoneNumber, MedicareExpirationDate)
VALUES
-- (123456789, 'John', 'Doe', '1990-01-15', '123 Main St', 'Cityville', 'Provinceville', 'A1B2C3', 'Canadian', 'john.doe@example.com', '123-456-7890', '2025-01-15'),
(234567890, 'Jane', 'Smith', '1985-05-20', '456 Oak Ave', 'Townsville', 'Provinceville', 'X0Y1Z2', 'Canadian', 'jane.smith@example.com', '987-654-3210', '2024-06-30'),
-- (987654321, 'David', 'Wilson', '1992-07-25', '654 Cedar St', 'Hometown', 'Provinceville', 'K5L6M7', 'Canadian', 'david@example.com', '555-8765', '2026-07-25'),
(345678901, 'Michael', 'Johnson', '1982-11-10', '789 Maple Rd', 'Villagetown', 'Provinceville', 'M1N2O3', 'Canadian', 'michael.johnson@example.com', '555-789-1234', '2023-09-30');

-- Ministry table
INSERT INTO Ministry (MinistryID, HeadOfficeFacilityAddress, MinisterOfEducation)
VALUES
(1, '456 Admin Blvd', 123456789);

-- Facility table
INSERT INTO Facility (FacilityID, MinistryID, Name, Address, City, Province, CapOnEmployees, PostalCode, Phone, WebAddress, Type)
VALUES
(1, 1, 'Head Office', '456 Admin Blvd', 'Cityville', 'Provinceville', 1000, 'A1B2C3', '555-123-4567', 'http://headoffice.example.com', 'Management'),
(2, 1, 'City High School', '123 School St', 'Cityville', 'Provinceville', 800, 'D4E5F6', '555-987-6543', 'http://cityhighschool.example.com', 'Education');

-- Educational table
INSERT INTO Educational (FacilityID, MinistryID, Principle, Type)
VALUES
(2, 1, 123456789, 'High School');

-- Infection table
INSERT INTO Infection (DateOfInfection, MedicareCardNumber, InfectionType)
VALUES
('2022-03-10', 123456789, 'COVID-19');
-- ('2023-01-25', 234567890, 'SARS-Cov-2 Variant');	-- used in #5

-- Vaccination table
INSERT INTO Vaccination (DoseNumber, Date, Type, MedicareCardNumber)
VALUES
-- (1, '2021-05-20', 'Pfizer', 123456789),	-- used in #4
(1, '2022-03-01', 'Moderna', 234567890),
(2, '2022-06-15', 'Moderna', 234567890),
(1, '2023-03-15', 'Pfizer', 345678901);

-- Student table
INSERT INTO Student (MedicareCardNumber, FacilityID, MinistryID, Level)
VALUES
(987654321, 2, 1, 'High School');

-- Employee table
INSERT INTO Employee (Role, MedicareCardNumber, FacilityID, MinistryID)
VALUES
('Teacher', 234567890, 2, 1),
('Administrator', 345678901, 1, 1);

-- Management table
INSERT INTO Management (FacilityID, MinistryID, PresidentID, HeadOffice)
VALUES
(1, 1, 123456789, true);

-- SchoolsAttendedByStudents table
INSERT INTO SchoolsAttendedByStudents (MedicareCardNumber, FacilityID, MinistryID, Level, StartDate, EndDate)
VALUES
(987654321, 2, 1, 'High School', '2020-09-01', '2022-06-30');

-- WorkplaceForEachEmployee table
INSERT INTO WorkplaceForEachEmployee (Role, MedicareCardNumber, FacilityID, MinistryID, StartDate, EndDate)
VALUES
('Teacher', 234567890, 2, 1, '2021-01-15', NULL),
('Administrator', 345678901, 1, 1, '2022-02-01', NULL);

-- EmployeeSchedule table
INSERT INTO EmployeeSchedule (FacilityID, MinistryID, MedicareCardNumber, Date, StartTime, EndTime)
VALUES
(2, 1, 234567890, '2023-08-10', '08:00:00', '16:00:00');
