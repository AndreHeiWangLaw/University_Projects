--comp 353 a2

-- a)

SELECT 
    m.memberId,
    m.firstName,
    m.lastName,
    m.middleName,
    m.dateOfBirth,
    m.address,
    m.gender,
    m.phoneNumber,
    m.email,
    m.socialInsuranceNumber,
    m.startDateMembership,
    e.jobTitle
FROM 
    Member m -- 'Member' as 'm'
JOIN
    Employee e ON m.memberId = e.memberId -- 'Employee' as 'e' on matching member ID
WHERE 
    m.memberType = 'client' AND-- select members with a memberType of 'client' 
    m.memberId IN ( -- filter members 
        SELECT memberId 
		FROM Donation -- retrieve member IDs from the table 'Donation'
        INTERSECT -- INTERSECT operator, combine the results 
        SELECT memberId 
		FROM Employee -- retrieve member IDs from the table 'Employee'
		WHERE salary IS NULL OR salary = 0 -- salary is NULL or 0
    );



-- b)
SELECT
    e.expenseId,
    m.firstName,
    m.lastName,
    e.date,
    e.amount,
    e.type,
    e.description
FROM
    Expenses e -- 'Expenses' as 'e'
JOIN
    Member m ON e.presidentId = m.memberId -- 'Member' as 'm' on matching president IDs
WHERE
    EXTRACT(MONTH FROM e.date) = 6 AND-- filter, result in the month of June = 6
    EXTRACT(YEAR FROM e.date) = 2023; -- filter, result from the year 2023


-- c)

SELECT
    s.salesId,
    s.date,
    m.firstName,
    m.lastName,
    p.description,
    p.sellingPrice,
    p.weight
FROM
    Sales s	-- 'Sales' as 's'
JOIN
    Member m ON s.memberId = m.memberId -- 'Member' as 'm' on matching member IDs
JOIN
    SalesItems si ON s.salesId = si.salesId -- 'SalesItems' as 'si' on matching Sales IDs
JOIN
    Product p ON si.productId = p.productId -- 'Product' as 'p' on matching Product IDs
WHERE
    EXTRACT(MONTH FROM s.date) = 6 AND -- filter, result in the month of June = 6
    EXTRACT(YEAR FROM s.date) = 2023 AND -- filter, result from the year 2023
    m.address LIKE '%Brossard%' -- filter, result containing 'Brossard'
ORDER BY
    s.salesId ASC, m.firstName ASC, m.lastName ASC, p.weight ASC;	-- order



-- d)
SELECT
    s.salesId,
    s.date,
    p.description,
    p.sellingPrice,
    p.weight
FROM
    Sales s -- 'Sales' as 's'
JOIN
    SalesItems si ON s.salesId = si.salesId -- 'SalesItems' as 'si' on matching Sales IDs
JOIN
    Product p ON si.productId = p.productId -- 'Product' as 'p' on matching Product IDs
WHERE
    EXTRACT(MONTH FROM s.date) = 6 AND -- filter, result in the month of June = 6
	EXTRACT(YEAR FROM s.date) = 2023 AND-- filter, result from the year 2023
    NOT EXISTS (	-- filter based on abscence
        SELECT 1 	-- placeholder, return value 1
		FROM Member m
        WHERE s.memberId = m.memberId AND 
		m.memberType = 'client'	-- member is a client that purchased the product
    )
ORDER BY
    s.salesId ASC, p.sellingPrice ASC;


-- e)

SELECT
    m.firstName,
    m.lastName,
    MIN(s.date) AS firstSaleDate,
    MAX(s.date) AS lastSaleDate,
    SUM(s.amount) AS totalAmountOfSales		-- total 
FROM
    Member m
JOIN
    Sales s ON m.memberId = s.memberId -- 'Sales' as 's' on matching Mamber IDs
WHERE
    m.memberType = 'client'	-- member is a client that purchased the product
GROUP BY
    m.memberId, m.firstName, m.lastName
HAVING
    SUM(s.amount) >= 1000	-- at least 1000$
ORDER BY	
    totalAmountOfSales;		-- sort based on total 



