<?php


	SELECT
		F.Province,
		F.Name AS SchoolName,
		F.CapOnEmployees AS Capacity,
		COUNT(DISTINCT I.MedicareCardNumber) AS InfectedTeachers,
		COUNT(DISTINCT S.MedicareCardNumber) AS InfectedStudents
	FROM Facility F
	LEFT JOIN 
		Employee e ON F.FacilityID = e.FacilityID AND 
		F.MinistryID = e.MinistryID
	LEFT JOIN 
		Infection I ON e.MedicareCardNumber = I.MedicareCardNumber AND 
		I.DateOfInfection BETWEEN '$twoWeeksAgo' AND '$currentDate'
	LEFT JOIN 
		Student S ON F.FacilityID = S.FacilityID AND 
		F.MinistryID = S.MinistryID
	LEFT JOIN 
		Infection st ON S.MedicareCardNumber = st.MedicareCardNumber AND 
		st.DateOfInfection BETWEEN '$twoWeeksAgo' AND '$currentDate'
	WHERE 
		F.Type = 'Education' AND
		I.InfectionType = 'COVID-19' AND
		S.Level = 'High School' 
	GROUP BY 
		F.FacilityID, F.MinistryID, F.Province, F.Name, F.CapOnEmployees
	ORDER BY 
		F.Province ASC, InfectedTeachers ASC;
?>