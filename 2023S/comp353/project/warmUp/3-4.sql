
SELECT DISTINCT
    p.personfirstName AS first_name,
    p.personlastName AS last_name,
    p.personDateOfBirth AS date_of_birth,
    p.personMedicareCardNumber AS medicare_card_number,
    p.personMedicareExpiryDate AS medicare_expiry_date,
    p.personPhoneNumber AS telephone_number,
    p.personEmailAddress AS email_address
FROM
    person p
    JOIN infection i ON p.personId = i.personId
    JOIN facility f ON p.personId = f.personID
WHERE
    p.personRole = 'student'
	AND i.infectionType = 'alpha' OR i.infectionType = 'beta' OR i.infectionType = 'gamma' OR i.infectionType = 'delta' OR i.infectionType = 'omicron'
    AND p.personMedicareExpiryDate < '2023-07-22'
	AND i.infectionType IN ('alpha', 'beta', 'gamma', 'delta', 'omicron')
    AND f.facilityName = 'Rosemont elementary school'
ORDER BY
    p.personMedicareExpiryDate ASC;





SELECT DISTINCT
    p.personfirstName,
    p.personlastName,
    p.personDateOfBirth,
    i.infectionType,
    i.infectionDate,
    p.personPhoneNumber,
    p.personEmailAddress
FROM
    person p
WHERE
    p.personRole = 'student'
    AND p.personId IN (
        SELECT 
            personId 
        FROM 
            infection i
        WHERE 
            i.infectionDate >= DATE_SUB(CURDATE(), INTERVAL 2 WEEK)
			AND i.infectionType IN ('alpha', 'beta', 'gamma', 'delta', 'omicron')
    )
	AND p.personId IN (
        SELECT 
            personId 
        FROM 
            facility f
        WHERE 
            f.facilityCity = 'Laval'
			AND f.facilityType = 'educational'
    )
GROUP BY
    p.personId
HAVING
    COUNT(i.infectionId) >= 2
ORDER BY
    p.personfirstName ASC,
    p.personlastName ASC,
    i.infectionDate ASC;




SELECT DISTINCT
    p.personfirstName AS first_name,
    p.personlastName AS last_name,
    p.personDateOfBirth AS date_of_birth,
    i.infectionType AS infection_type,
    i.infectionDate AS infection_date,
    p.personPhoneNumber AS telephone_number,
    p.personEmailAddress AS email_address
FROM
    person p
    JOIN infection i ON p.personId = i.personId
    JOIN facility f ON p.personId = f.personID
WHERE
    p.personRole = 'student'
    AND f.facilityType = 'educational'
    AND f.facilityCity = 'Laval'
    AND i.infectionDate >= DATE_SUB(CURDATE(), INTERVAL 2 WEEK)
	AND i.infectionType = 'alpha' OR i.infectionType = 'beta' OR i.infectionType = 'gamma' OR i.infectionType = 'delta' OR i.infectionType = 'omicron'
GROUP BY
    p.personId, i.infectionType, i.infectionDate
HAVING
    COUNT(i.infectionId) >= 2
ORDER BY
    p.personfirstName ASC,
    p.personlastName ASC,
    i.infectionDate ASC;






SELECT DISTINCT
    p.personfirstName,
    p.personlastName,
    p.personDateOfBirth,
    i.infectionType,
    i.infectionDate,
    p.personPhoneNumber,
    p.personEmailAddress
FROM 
    person p
JOIN 
    infection i ON p.personId = i.personId
JOIN 
    facility f ON p.personId = f.personId
WHERE 
    p.personRole = 'student' AND
    f.facilityCity = 'Laval' AND
    i.infectionDate >= DATE_SUB(CURDATE(), INTERVAL 2 WEEK) 
HAVING COUNT(i.infectionId) >= 2
ORDER BY
    p.personfirstName ASC,
    p.personlastName ASC,
    i.infectionDate ASC;
