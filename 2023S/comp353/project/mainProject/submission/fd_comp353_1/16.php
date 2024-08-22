<?php
$host = 'fdc353.encs.concordia.ca';
$user = 'fdc353_1';
$db = 'fdc353_1';

$conn = mysqli_connect($host, $user, '353psswd', $db);

if (!$conn) {
    die("Connection failed: " . mysqli_connect_error());
}

$sql = "
SELECT 
    min.MinistryID AS MinistryID,
    p.FirstName AS MinisterFirstName,
    p.LastName AS MinisterLastName,
    p.City AS MinisterCity,
    COUNT(DISTINCT m.FacilityID) AS TotalManagementFacilities,
    COUNT(DISTINCT e.FacilityID) AS TotalEducationalFacilities
FROM Ministry AS min
JOIN Person AS p ON min.MinisterOfEducation = p.MedicareCardNumber
LEFT JOIN Facility AS f ON min.MinistryID = f.MinistryID
LEFT JOIN Management AS m ON f.FacilityID = m.FacilityID AND f.MinistryID = m.MinistryID AND min.MinisterOfEducation = m.PresidentID
LEFT JOIN Educational AS e ON f.FacilityID = e.FacilityID AND f.MinistryID = e.MinistryID
GROUP BY min.MinistryID
HAVING COUNT(DISTINCT m.FacilityID) > 0
ORDER BY p.City ASC, TotalEducationalFacilities DESC;
";

$result = $conn->query($sql);

if ($result->num_rows > 0) {
    echo "<table><tr><th>FirstName</th><th>LastName</th><th>City</th><th>Management Facilities</th><th>Educational Facilities</th></tr>";
    while($row = $result->fetch_assoc()) {
        echo "<tr><td>" . $row["MinisterFirstName"]. "</td><td>" . $row["MinisterLastName"]. "</td><td>" . $row["MinisterCity"]. "</td><td>" . $row["TotalManagementFacilities"]. "</td><td>" . $row["TotalEducationalFacilities"]. "</td></tr>";
    }
    echo "</table>";
} else {
    echo "0 results";
}

$conn->close();
?>
