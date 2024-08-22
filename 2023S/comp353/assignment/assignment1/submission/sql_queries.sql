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



-- 12


-- 13


-- 14


-- 15


-- 16


-- 17


-- 18


-- 19


-- 20





