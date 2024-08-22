INSERT INTO person (
    personfirstName,personlastName, personDateOfBirth, personMedicareCardNumber, personMedicareExpiryDate,
    personPhoneNumber, personAddress, personCity, personProvince, personPostalCode, personCitizenship,
    personEmailAddress, personRole
) VALUES
    ('John', 'Doe', '1990-05-15', '1234567890', '2018-05-15', '555-123-4567', '123 Main St', 'Anytown', 'Alberta', 'A1B 2C3', 'citizen by birth', 'john.doe@example.com', 'student'),
    ('Jane', 'Smith', '1985-11-02', '9876543210', '2028-10-31', '444-555-7890', '456 Elm St', 'Sometown', 'Ontario', 'X0X 1Y2', 'citizen by naturalization', 'jane.smith@example.com', 'student'),
    ('Michael', 'Johnson', '2001-08-20', '1357924680', '2019-08-20', '777-888-9999', '789 Oak Ave', 'Anothercity', 'British Columbia', 'Z9Z 9Z9', 'non-citizen', 'michael.johnson@example.com', 'student'),
    ('Emily', 'Lee', '1998-03-10', '4567890123', '2020-12-31', '222-333-4444', '321 Pine Rd', 'Cityville', 'Manitoba', 'M4M 3M3', 'citizen by birth', 'emily.lee@example.com', 'student'),
    ('David', 'Wong', '1982-12-25', '7890123456', '2023-12-31', '999-888-7777', '987 Birch Blvd', 'Townsville', 'Quebec', 'Q7Q 7Q7', 'citizen by naturalization', 'david.wong@example.com', 'student'),
    ('Sarah', 'Kim', '1995-06-30', '2468135790', '2024-06-30', '111-222-3333', '654 Cedar Lane', 'Suburbia', 'Ontario', 'K1K 1K1', 'non-citizen', 'sarah.kim@example.com', 'employee'),
    ('William', 'Martin', '2000-02-18', '1357902468', '2026-02-18', '777-777-5555', '852 Willow Cres', 'Metroville', 'Nova Scotia', 'N5N 5N5', 'citizen by birth', 'william.martin@example.com', 'student'),
    ('Olivia', 'Garcia', '1992-07-05', '5790134682', '2025-07-05', '444-999-6666', '963 Rose Lane', 'Townburg', 'Saskatchewan', 'S8S 8S8', 'citizen by naturalization', 'olivia.garcia@example.com', 'employee'),
    ('James', 'Thompson', '1994-09-12', '1029384756', '2023-09-12', '777-444-5555', '753 Maple Ave', 'Countryville', 'Prince Edward Island', 'P1P 1P1', 'citizen by birth', 'james.thompson@example.com', 'student'),
    ('Sophia', 'Allen', '1999-11-22', '1357924680', '2026-11-22', '888-555-3333', '159 Oak St', 'Villagetown', 'New Brunswick', 'B2B 2B2', 'non-citizen', 'sophia.allen@example.com', 'employee');

	
INSERT INTO vaccination (vaccinationDate, doseNumber, vaccinationType, personId)
VALUES
	('2023-03-21', 1, 'Pfizer', 1),
	('2023-03-22', 2, 'Pfizer', 1),
	('2023-03-23', 1, 'Moderna', 2),
	('2023-03-24', 2, 'Moderna', 2),
	('2023-03-25', 1, 'AstraZeneca', 3),
	('2023-03-26', 2, 'AstraZeneca', 3),
	('2023-03-27', 1, 'Johnson & Johnson', 4),
	('2023-03-28', 1, 'Pfizer', 5),
	('2023-03-29', 2, 'Pfizer', 5),
	('2023-03-30', 1, 'Moderna', 6);


INSERT INTO facility (facilityName, facilityAddress, facilityCity, facilityProvince, facilityPostalCode,
    facilityPhoneNumber, facilityWebAddress, capOnEmployees, facilityType, facilityPresident, personID) 
VALUES
    ('Rosemont elementary school', '123 Main St', 'Laval', 'Alberta', 'A1B 2C3', '555-123-4567', 'www.abcmanagement.com', 50, 'educational', 'President 1', 1),
    ('Rosemont elementary school', '456 Elm St', 'Laval', 'Ontario', 'X0X 1Y2', '444-555-7890', 'www.xyzeducenter.com', 100, 'educational', 'President 2', 2),
    ('Rosemont elementary school', '789 Oak Ave', 'Laval', 'British Columbia', 'Z9Z 9Z9', '777-888-9999', 'www.pqrms.com', 30, 'educational', 'President 3', 3),
    ('Rosemont elementary school', '321 Pine Rd', 'Laval', 'Manitoba', 'M4M 3M3', '222-333-4444', 'www.lmneduinst.com', 80, 'educational', 'President 4', 4),
    ('Rosemont elementary school', '987 Birch Blvd', 'Townsville', 'Quebec', 'Q7Q 7Q7', '999-888-7777', 'www.efgservices.com', 25, 'educational', 'President 5', 5),
    ('JKL Learning Center', '654 Cedar Lane', 'Suburbia', 'Ontario', 'K1K 1K1', '111-222-3333', 'www.jkllearningcenter.com', 60, 'educational', 'President 6', 6),
    ('XYZ Management', '852 Willow Cres', 'Metroville', 'Nova Scotia', 'N5N 5N5', '777-777-5555', 'www.xyzmanagement.com', 40, 'management', 'President 7', 7),
    ('ABC Educational Services', '963 Rose Lane', 'Townburg', 'Saskatchewan', 'S8S 8S8', '444-999-6666', 'www.abceduservices.com', 90, 'educational', 'President 8', 8),
    ('PQR Management Academy', '753 Maple Ave', 'Countryville', 'Prince Edward Island', 'P1P 1P1', '777-444-5555', 'www.pqrma.com', 35, 'management', 'President 9', 9),
    ('LMN Learning Solutions', '159 Oak St', 'Villagetown', 'New Brunswick', 'B2B 2B2', '888-555-3333', 'www.lmnlearningsolutions.com', 70, 'educational', 'President 10', 10);


INSERT INTO infection (infectionDate, infectionType, personId) 
VALUES
    ('2023-01-08', 'alpha', 1),
    ('2023-01-10', 'beta', 1),
    ('2023-01-12', 'gamma', 1),
    ('2023-02-24', 'delta', 2),
    ('2023-02-25', 'omicron', 2),
    ('2023-02-26', 'alpha', 3),
    ('2023-03-05', 'beta', 4),
    ('2023-03-10', 'gamma', 8),
    ('2023-03-15', 'delta', 9),
    ('2023-10-20', 'omicron', 10);
	

INSERT INTO ministry (ministryHeadName, ministryFacilityId)
VALUES
    (1, 1),
    (1, 3),
    (1, 5),
    (3, 2),
    (3, 4),
    (2, 6),
    (3, 8),
    (3, 10),
    (1, 7),
    (1, 9);