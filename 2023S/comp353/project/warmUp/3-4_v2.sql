-- #3
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
JOIN 
    infection i ON p.personId = i.personId
WHERE 
	i.infectionType IN ('alpha', 'beta', 'gamma', 'delta', 'omicron') 
    AND p.personRole = 'student' 
    AND p.personMedicareExpiryDate <= CURDATE() 
    AND p.personId IN (
        SELECT 
            personId 
        FROM 
            facility f
        WHERE 
            f.facilityName = 'Rosemont elementary school'
    )
ORDER BY 
    p.personMedicareExpiryDate ASC;
	
-- expected results: John Doe, Micheal Johnson, Emily Lee



-- #4
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
JOIN 
	infection i ON p.personId = i.personId
WHERE
    p.personRole = 'student'
    AND i.infectionDate <= DATE_SUB(CURDATE(), INTERVAL 2 WEEK)
	AND i.infectionType IN ('alpha', 'beta', 'gamma', 'delta', 'omicron')
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
	
-- expected results: Jane Smith, John Doe


