-- sql populate the tables

-- Person Table:
INSERT INTO Person (MedicareCardNumber, FirstName, LastName, DateOfBirth, Address, City, Province, PostalCode, Citizenship, EmailAddress, TelephoneNumber, MedicareExpirationDate)
VALUES
(123456789, 'John', 'Doe', '1990-05-15', '123 Main St', 'Cityville', 'Provinceville', 'A1B 2C3', 'Canadian', 'john@example.com', '555-1234', '2025-05-15'),
(234567890, 'Jane', 'Smith', '1995-08-20', '456 Elm St', 'Townsville', 'Provinceville', 'X1Y 2Z3', 'Canadian', 'jane@example.com', '555-5678', '2024-08-20'),
(345678901, 'Michael', 'Johnson', '1988-02-10', '789 Oak St', 'Villagetown', 'Provinceville', 'M1N 2O3', 'Canadian', 'michael@example.com', '555-9876', '2023-02-10'),
(456789012, 'Emily', 'Brown', '2000-11-30', '987 Pine St', 'Hamletville', 'Provinceville', 'P9Q 8R7', 'Canadian', 'emily@example.com', '555-4321', '2022-11-30'),
(567890123, 'David', 'Wilson', '1992-07-25', '654 Cedar St', 'Hometown', 'Provinceville', 'K5L 6M7', 'Canadian', 'david@example.com', '555-8765', '2026-07-25');


-- Ministry Table:
INSERT INTO Ministry (MinistryID, HeadOfficeFacilityAddress, MinisterOfEducation)
VALUES
(1, '100 Ministry Ave', 123456789),
(2, '200 Ministry Rd', 234567890),
(3, '300 Ministry Ln', 345678901),
(4, '400 Ministry St', 456789012),
(5, '500 Ministry Blvd', 567890123);


-- Facility Table:
INSERT INTO Facility (FacilityID, MinistryID, Name, Address, City, Province, CapOnEmployees, PostalCode, Phone, WebAddress, Type)
VALUES
(1, 1, 'High School A', '123 School Rd', 'Cityville', 'Provinceville', 100, 'A1B 2C3', '555-1000', 'http://www.schoola.com', 'High School'),
(2, 1, 'High School B', '456 Learning Ln', 'Townsville', 'Provinceville', 150, 'X1Y 2Z3', '555-2000', 'http://www.schoolb.com', 'High School'),
(3, 2, 'Elementary School X', '789 Elementary St', 'Villagetown', 'Provinceville', 80, 'M1N 2O3', '555-3000', 'http://www.schoolx.com', 'Elementary School'),
(4, 2, 'Elementary School Y', '987 Primary Ave', 'Hamletville', 'Provinceville', 70, 'P9Q 8R7', '555-4000', 'http://www.schooly.com', 'Elementary School'),
(5, 3, 'College Z', '654 Campus Rd', 'Hometown', 'Provinceville', 200, 'K5L 6M7', '555-5000', 'http://www.collegez.com', 'College');


-- Educational Table:
INSERT INTO Educational (FacilityID, MinistryID, Principle, Type)
VALUES
(1, 1, 123456789, 'High School'),
(2, 1, 234567890, 'High School'),
(3, 2, 345678901, 'Elementary School'),
(4, 2, 456789012, 'Elementary School'),
(5, 3, 567890123, 'College');


-- Infection Table:
INSERT INTO Infection (DateOfInfection, MedicareCardNumber, InfectionType)
VALUES
('2023-03-10', 123456789, 'Flu'),
('2022-09-05', 234567890, 'Cold'),
('2021-12-20', 345678901, 'COVID-19'),
('2022-06-15', 456789012, 'Strep Throat'),
('2023-01-30', 567890123, 'Influenza');


-- Vaccination Table:
INSERT INTO Vaccination (DoseNumber, Date, Type, MedicareCardNumber)
VALUES
(1, '2022-02-10', 'Flu Shot', 123456789),
(1, '2022-08-25', 'COVID-19', 234567890),
(2, '2023-01-15', 'COVID-19', 234567890),
(1, '2021-11-05', 'Tetanus', 345678901),
(1, '2023-04-20', 'Measles', 456789012);


-- Student Table:
INSERT INTO Student (MedicareCardNumber, FacilityID, MinistryID, Level)
VALUES
(123456789, 1, 1, 'High School'),
(234567890, 2, 1, 'High School'),
(345678901, 3, 2, 'Elementary School'),
(456789012, 4, 2, 'Elementary School'),
(567890123, 5, 3, 'College');


-- Employee Table:
INSERT INTO Employee (Role, MedicareCardNumber, FacilityID, MinistryID)
VALUES
('Teacher', 987654321, 2, 1),
('Principal', 876543210, 1, 1),
('Custodian', 765432109, 3, 2),
('Instructor', 654321098, 5, 3),
('Librarian', 543210987, 4, 2);


-- Management Table:
INSERT INTO Management (FacilityID, MinistryID, PresidentID, HeadOffice)
VALUES
(1, 1, 876543210, TRUE),
(2, 1, 123456789, FALSE),
(3, 2, 345678901, TRUE),
(4, 2, 456789012, FALSE),
(5, 3, 567890123, TRUE);


-- SchoolsAttendedByStudents Table:
INSERT INTO SchoolsAttendedByStudents (MedicareCardNumber, FacilityID, MinistryID, Level, StartDate, EndDate)
VALUES
(123456789, 1, 1, 'High School', '2019-09-01', '2023-06-15'),
(234567890, 2, 1, 'High School', '2018-08-15', '2022-05-30'),
(345678901, 3, 2, 'Elementary School', '2013-09-01', '2020-06-30'),
(456789012, 4, 2, 'Elementary School', '2010-08-15', '2019-05-30'),
(567890123, 5, 3, 'College', '2022-09-01', '2026-05-15');


-- WorkplaceForEachEmployee Table:
INSERT INTO WorkplaceForEachEmployee (Role, MedicareCardNumber, FacilityID, MinistryID, StartDate, EndDate)
VALUES
('Teacher', 987654321, 2, 1, '2020-01-15', NULL),
('Principal', 876543210, 1, 1, '2019-05-10', NULL),
('Custodian', 765432109, 3, 2, '2022-03-20', NULL),
('Instructor', 654321098, 5, 3, '2021-07-01', NULL),
('Librarian', 543210987, 4, 2, '2023-02-10', NULL);


-- EmployeeSchedule Table:
INSERT INTO EmployeeSchedule (FacilityID, MinistryID, MedicareCardNumber, Date, StartTime, EndTime)
VALUES
(1, 1, 876543210, '2023-08-10', '09:00:00', '16:00:00'),
(2, 1, 123456789, '2023-08-10', '08:30:00', '15:30:00'),
(3, 2, 345678901, '2023-08-10', '10:00:00', '18:00:00'),
(4, 2, 456789012, '2023-08-10', '07:30:00', '14:30:00'),
(5, 3, 567890123, '2023-08-10', '11:00:00', '19:00:00');

