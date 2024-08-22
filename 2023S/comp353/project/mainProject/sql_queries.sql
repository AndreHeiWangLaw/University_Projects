-- SQL, DDL and DML commands


-- 1


-- 2


-- 3



-- 4
-- Create 
INSERT INTO Vaccination (DoseNumber, Date, Type, MedicareCardNumber)
VALUES (1, '2021-05-20', 'Pfizer', 123456789);


-- Edit 
UPDATE Vaccination SET Type = 'Moderna' WHERE MedicareCardNumber = 123456789 AND DoseNumber = 1;

-- Display 
SELECT * FROM Vaccination;

-- Delete 
DELETE FROM Vaccination WHERE MedicareCardNumber = 123456789 AND DoseNumber = 1;


-- 5
-- Create 
INSERT INTO Infection (DateOfInfection, MedicareCardNumber, InfectionType)
VALUES ('2023-01-25', 234567890, 'SARS-Cov-2 Variant');

-- Edit 
UPDATE Infection SET InfectionType = 'COVID-19' WHERE MedicareCardNumber = 234567890 AND DoseNumber  = 1;

-- Display 
SELECT * FROM Infection;

-- Delete 
DELETE FROM Infection WHERE MedicareCardNumber = 234567890 AND DoseNumber  = 1;


-- 6

-- read #19 and registering paragraph


-- 7

-- read #19 and secheduling paragraph

-- 8
SELECT
    F.Name AS FacilityName,
    F.Address,
    F.City,
    F.Province,
    F.PostalCode,
    F.Phone,
    F.WebAddress,
    F.Type,
    F.CapOnEmployees AS Capacity,
    P.FirstName AS PresidentFirstName,
    P.LastName AS PresidentLastName,
    COUNT(E.MedicareCardNumber) AS NumberOfEmployees
FROM
    Facility AS F
LEFT JOIN
    Management AS M ON F.FacilityID = M.FacilityID AND F.MinistryID = M.MinistryID	-- primary key
LEFT JOIN
    Employee AS E ON F.FacilityID = E.FacilityID AND F.MinistryID = E.MinistryID
    AND E.Role = 'Teacher'
LEFT JOIN
    Person AS P ON M.PresidentID = P.MedicareCardNumber
GROUP BY
    F.FacilityID, F.MinistryID
ORDER BY
    F.Province ASC,
    F.City ASC,
    F.Type ASC,
    NumberOfEmployees ASC;


-- 9
SELECT
    P.FirstName,
    P.LastName,
    W.StartDate,
    P.DateOfBirth,
    P.MedicareCardNumber,
    P.TelephoneNumber,
    P.Address,
    P.City,
    P.Province,
    P.PostalCode,
    P.Citizenship,
    P.EmailAddress
FROM
    Person AS P
JOIN
    WorkplaceForEachEmployee AS W ON P.MedicareCardNumber = W.MedicareCardNumber
WHERE
    W.FacilityID = 1	-- change id to target specific facility
    AND W.EndDate IS NULL -- Null = currently working
ORDER BY
    W.Role ASC,
    P.FirstName ASC,
    P.LastName ASC;


-- 10
SELECT
    F.Name AS FacilityName,
    DAYOFYEAR(ES.Date) AS DayOfYear,
    ES.StartTime,
    ES.EndTime
FROM
    EmployeeSchedule AS ES
JOIN
    Facility AS F ON ES.FacilityID = F.FacilityID
WHERE
    ES.MedicareCardNumber = 234567890	-- cahnge to select other employee
    AND ES.Date >= '2023-08-10'	-- example of start date
    AND ES.Date <= '2023-08-15'	-- example of end date
ORDER BY
    F.Name ASC,
    DayOfYear ASC,
    ES.StartTime ASC;


-- 11
INSERT INTO Person (MedicareCardNumber, FirstName, LastName, DateOfBirth, Address, City, Province, PostalCode, Citizenship, EmailAddress, TelephoneNumber, MedicareExpirationDate)
VALUES
(1001001001, 'Bob', 'Bobby', '1989-11-10', '79 Maple Rd', 'Towntown', 'Provinceprovince', 'M0M1O3', 'Canadian', 'bob.bobby@example.com', '555-001-1234', '2024-09-30'),
(1001001002, 'Carl', 'Carly', '1999-01-14', '70 Toad Rd', 'Poptown', 'Princeprovince', 'Y0M1O3', 'Canadian', 'carl.carly@example.com', '505-101-1234', '2025-07-30'),
(1001001003, 'Max', 'Maxy', '1991-12-11', '709 Leaf Rd', 'Sintown', 'Provincesin', 'R1R1O3', 'Canadian', 'max.maxy@example.com', '555-101-3333', '2022-09-30'),
(1001001004, 'Son', 'Sonny', '1973-04-04', '970 Apple Rd', 'Clowntown', 'Provinceclown', 'Q0Q133', 'Canadian', 'son.sonny@example.com', '505-101-444', '2024-01-19'),
(1001001005, 'Ted', 'Teddy', '1992-10-21', '70 Rare Rd', 'Raretown', 'Provincerare', 'A1R223', 'Canadian', 'ted.teddy@example.com', '500-000-3333', '2022-04-20'),
(1001001006, 'Red', 'Reddy', '1975-02-14', '127 Gold Rd', 'Goldtown', 'Provincegold', 'Q0E121', 'Canadian', 'red.reddy@example.com', '505-333-555', '2026-10-12'),
(1001001007, 'Pile', 'Piley', '1973-04-04', '970 Apple Rd', 'Clowntown', 'Provincerile', 'H0P173', 'Canadian', 'pile.piley@example.com', '505-101-444', '2022-11-14'),
(1001001008, 'Ron', 'Ronny', '1998-12-11', '120 Tim Rd', 'Timtown', 'Provincetim', 'O1H923', 'Canadian', 'ron.ronny@example.com', '500-583-4852', '2023-07-10'),
(1001001009, 'Gord', 'Gordy', '1999-03-24', '17 Rose Rd', 'Rosetowm', 'Provincerose', 'H0L161', 'Canadian', 'gord.gordy@example.com', '505-195-1357', '2022-07-29'),
(1001001010, 'Lil', 'Lilly', '2001-12-24', '17 Lily Rd', 'Lilytown', 'Provincelily', 'A0L161', 'Canadian', 'lil.lilly@example.com', '555-8890-9345', '2026-07-29');


INSERT INTO Ministry (MinistryID, HeadOfficeFacilityAddress, MinisterOfEducation)
VALUES
(10, '456 Silver Blvd', 1001001006),
(11, '456 Bronze Blvd', 1001001007);

INSERT INTO Facility (FacilityID, MinistryID, Name, Address, City, Province, CapOnEmployees, PostalCode, Phone, WebAddress, Type)
VALUES
(10, 10, 'ABC High School', '123 Admin Blvd', 'CityABC', 'ProvinceABC', 1111, 'B1B2C3', '555-123-0000', 'http://ABCHighSchool.example.com', 'Education'),
(11, 11, 'BEST High School', '321 School St', 'CityBest', 'ProvinceBEST', 820, 'E4E5E6', '555-000-6543', 'http://BESTHighSchool.example.com', 'Education');
-- (12, 12, 'Silver High School', '123 Silver St', 'Raretown', 'Provincerare', 100, 'G1G2C1', '555-443-3344', 'http://SilverHighSchool.example.com', 'Education'),
-- (13, 13, 'Bronze High School', '321 Bronze St', 'Goldtown', 'Provincegold', 250, 'B4B1B6', '555-667-7776', 'http://BronzeHighSchool.example.com', 'Education');

INSERT INTO Educational (FacilityID, MinistryID, Principle, Type)
VALUES
(10, 10, 1001001005, 'High School'),
(11, 11, 1001001006, 'High School');

INSERT INTO Employee (Role, MedicareCardNumber, FacilityID, MinistryID)
VALUES
('Teacher', 1001001001, 10, 10),
('Teacher', 1001001002, 10, 10),
('Teacher', 1001001003, 10, 10),
('Teacher', 1001001004, 11, 11),
('Teacher', 1001001005, 11, 11);

INSERT INTO Student (MedicareCardNumber, FacilityID, MinistryID, Level)
VALUES
(1001001008, 10, 10, 'High School'),
(1001001009, 11, 11, 'High School'),
(1001001010, 11, 11, 'High School');

INSERT INTO Infection (DateOfInfection, MedicareCardNumber, InfectionType)
VALUES
('2023-08-01', 1001001001, 'COVID-19'),
('2023-08-02', 1001001002, 'COVID-19'),
('2023-08-03', 1001001003, 'COVID-19'),
('2023-08-04', 1001001004, 'COVID-19'),
('2023-08-05', 1001001005, 'COVID-19'),
('2023-08-06', 1001001006, 'COVID-19'),
('2023-08-07', 1001001007, 'COVID-19'),
('2023-08-08', 1001001008, 'COVID-19'),
('2023-08-09', 1001001009, 'COVID-19'),
('2023-08-10', 1001001010, 'COVID-19');

INSERT INTO Vaccination (DoseNumber, Date, Type, MedicareCardNumber)
VALUES 
(1, '2023-05-20', 'Pfizer', 1001001001),
(1, '2023-06-20', 'Moderna', 1001001002),
(1, '2023-07-20', 'AstraZeneca', 1001001008),
(1, '2023-08-20', 'Moderna', 1001001009);


-- 12


-- 13


-- 14


-- 15


-- 16


-- 17


-- 18


-- 19


-- 20





