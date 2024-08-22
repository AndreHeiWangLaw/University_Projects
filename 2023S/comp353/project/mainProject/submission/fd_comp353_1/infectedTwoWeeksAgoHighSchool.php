<?php

$host = 'fdc353.encs.concordia.ca';
$user = 'fdc353_1';
$pass = '353psswd';
$db = 'fdc353_1'; // replace 'your_database_name' with your actual database name

// Create connection
$conn = mysqli_connect($host, $user, $pass, $db);

// Check connection
if (!$conn) {
  die("Connection failed: " . mysqli_connect_error());
}


// dates details
$currentDate = date('Y-m-d');
$twoWeeksAgo = date('Y-m-d', strtotime('-2 weeks'));

// High schools of infected teachers/students in past 2 weeks
$query = "
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
    st.InfectionType = 'COVID-19' AND
		S.Level = 'High School' 
	GROUP BY 
		F.FacilityID, F.MinistryID, F.Province, F.Name, F.CapOnEmployees
	ORDER BY 
		F.Province ASC, InfectedTeachers ASC;";


$result = $conn->query($query);

// Check if the query executed successfully
if (!$result) {
    die("Query error: " . mysqli_error($conn));
}

?>
 
<!DOCTYPE html>
<html>
<head>
    <title>High School Infections</title>
</head>
<body>
<h1>High School Infections in the Past Two Weeks by COVID-19</h1>

<!-- show dates -->
<p>Current Date: <?php echo $currentDate; ?></p>
<p>Two Weeks Ago: <?php echo $twoWeeksAgo; ?></p>

<table border="1">
    <tr>
        <th>Province</th>
        <th>School Name</th>
        <th>Capacity</th>
        <th>Infected Teachers</th>
        <th>Infected Students</th>
    </tr>
    <?php
    while ($row = $result->fetch_assoc()) {
        echo "<tr>";
        echo "<td>" . $row['Province'] . "</td>";
        echo "<td>" . $row['SchoolName'] . "</td>";
        echo "<td>" . $row['Capacity'] . "</td>";
        echo "<td>" . $row['InfectedTeachers'] . "</td>";
        echo "<td>" . $row['InfectedStudents'] . "</td>";
        echo "</tr>";
    }
    ?>
</table>
<br><br>
<!-- return to index.php -->
<a href="infectionPage.php">Go Back to Infection Page</a>

</body>
</html>
