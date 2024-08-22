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

$query ="SELECT
			P.FirstName,
			P.LastName,
			I.DateOfInfection,
			F.Name AS FacilityName
		FROM 
			Infection AS I
		JOIN 
			Employee AS E ON I.MedicareCardNumber = E.MedicareCardNumber
		JOIN 
			Person AS P ON I.MedicareCardNumber = P.MedicareCardNumber
		JOIN 
			Facility AS F ON E.FacilityID = F.FacilityID AND 
			E.MinistryID = F.MinistryID
		WHERE 
			I.DateOfInfection >= DATE_SUB(CURRENT_DATE(), INTERVAL 2 WEEK) AND 
			E.Role = 'Teacher' AND
			I.InfectionType = 'COVID-19'
		GROUP BY
			P.FirstName
		ORDER BY 
			F.Name ASC, P.FirstName ASC";		

$result = $conn->query($query);
?>

<!DOCTYPE html>
<html>
<head>
    <title>Infected Teachers</title>
</head>
<body>
<h1>Infected Teachers in the Past Two Weeks by COVID-19</h1>

<!-- show dates -->
<p>Current Date: <?php echo $currentDate; ?></p>
<p>Two Weeks Ago: <?php echo $twoWeeksAgo; ?></p>


<table border="1">
    <tr>
        <th>First Name</th>
        <th>Last Name</th>
        <th>Date of Infection</th>
        <th>Facility Name</th>
    </tr>
    <?php
    while ($row = $result->fetch_assoc()) {
        echo "<tr>";
        echo "<td>" . $row['FirstName'] . "</td>";
        echo "<td>" . $row['LastName'] . "</td>";
        echo "<td>" . $row['DateOfInfection'] . "</td>";
        echo "<td>" . $row['FacilityName'] . "</td>";
        echo "</tr>";
    }
    ?>
</table>
<br><br>
<!-- return to index.php -->
<a href="infectionPage.php">Go Back to Infection Page</a>

</body>
</html>
