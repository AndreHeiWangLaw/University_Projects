<?php

$host = 'fdc353.encs.concordia.ca';
$user = 'fdc353_1';
$pass = '353psswd';
$db = 'fdc353_1';

$conn = new mysqli($host, $user, $pass, $db);

// Check connection
if ($conn->connect_error) {
    die("Connection failed: " . $conn->connect_error);
}

// Assuming you have the FacilityID for the specific facility
$facilityID = $_GET['facility_id']; // Make sure to validate and sanitize this input

$sql = "SELECT 
            E.Role, P.FirstName, P.LastName, W.StartDate, P.DateOfBirth, P.MedicareCardNumber, P.TelephoneNumber,
            P.Address, P.City, P.Province, P.PostalCode, P.Citizenship, P.EmailAddress
        FROM Employee AS E
        INNER JOIN Person AS P ON E.MedicareCardNumber = P.MedicareCardNumber
        INNER JOIN WorkplaceForEachEmployee AS W ON E.MedicareCardNumber = W.MedicareCardNumber
        WHERE W.FacilityID = $facilityID
        ORDER BY E.Role ASC, P.FirstName ASC, P.LastName ASC";

$result = $conn->query($sql);

if ($result->num_rows > 0) {
    echo "<table>
            <tr>
                <th>Role</th>
                <th>First Name</th>
                <th>Last Name</th>
                <th>Start Date</th>
                <th>Date of Birth</th>
                <th>Medicare Card Number</th>
                <th>Telephone Number</th>
                <th>Address</th>
                <th>City</th>
                <th>Province</th>
                <th>Postal Code</th>
                <th>Citizenship</th>
                <th>Email Address</th>
            </tr>";

    while ($row = $result->fetch_assoc()) {
        echo "<tr>
                <td>{$row['Role']}</td>
                <td>{$row['FirstName']}</td>
                <td>{$row['LastName']}</td>
                <td>{$row['StartDate']}</td>
                <td>{$row['DateOfBirth']}</td>
                <td>{$row['MedicareCardNumber']}</td>
                <td>{$row['TelephoneNumber']}</td>
                <td>{$row['Address']}</td>
                <td>{$row['City']}</td>
                <td>{$row['Province']}</td>
                <td>{$row['PostalCode']}</td>
                <td>{$row['Citizenship']}</td>
                <td>{$row['EmailAddress']}</td>
            </tr>";
    }
    echo "</table>";
} else {
    echo "No employees found in this facility.";
}

$conn->close();
?>
