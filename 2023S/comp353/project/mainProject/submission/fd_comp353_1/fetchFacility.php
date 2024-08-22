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
echo "Connected successfully<br>";

// Display Facility Information
if (isset($_POST['display_facility'])) {
    $FacilityName = $_POST['FacilityName'];
    $FacilityID = $_POST['FacilityID'];
    $MinistryID = $_POST['MinistryID'];

    $where_clause = "";

    if (!empty($FacilityName)) {
        $where_clause = "WHERE Name = '$FacilityName'";
    } elseif (!empty($FacilityID) && !empty($MinistryID)) {
        $where_clause = "WHERE FacilityID = '$FacilityID' AND MinistryID = '$MinistryID'";
    }

    // Fetch facility information
    $select_query = "SELECT * FROM Facility $where_clause";
    $result = $conn->query($select_query);

    if ($result->num_rows > 0) {
        while ($row = $result->fetch_assoc()) {
            echo "Facility ID: " . $row["FacilityID"] . "<br>";
            echo "Ministry ID: " . $row["MinistryID"] . "<br>";
            echo "Name: " . $row["Name"] . "<br>";
            echo "Address: " . $row["Address"] . "<br>";
            echo "City: " . $row["City"] . "<br>";
            echo "Province: " . $row["Province"] . "<br>";
            echo "Cap On Employees: " . $row["CapOnEmployees"] . "<br>";
            echo "Postal Code: " . $row["PostalCode"] . "<br>";
            echo "Phone: " . $row["Phone"] . "<br>";
            echo "Web Address: " . $row["WebAddress"] . "<br>";
            echo "Type: " . $row["Type"] . "<br>";
            echo "<br>";
        }
    } else {
        echo "No facilities found.";
    }
}


// Fetch All
if (isset($_POST['fetch_all'])) {
    // Database connection setup
    $host = 'fdc353.encs.concordia.ca';
    $user = 'fdc353_1';
    $pass = '353psswd';
    $db = 'fdc353_1'; // replace with your actual database name

    $conn = mysqli_connect($host, $user, $pass, $db);
    if (!$conn) {
        die("Connection failed: " . mysqli_connect_error());
    }

    $query = "SELECT * FROM Facility";
    $result = mysqli_query($conn, $query);

    if (mysqli_num_rows($result) > 0) {
        // Display the result as a table
        echo "<table border='1'>";
        echo "<tr><th>FacilityID</th><th>MinistryID</th><th>Name</th><th>Address</th><th>City</th><th>Province</th><th>CapOnEmployees</th><th>PostalCode</th><th>Phone</th><th>WebAddress</th><th>Type</th></tr>";
        while ($row = mysqli_fetch_assoc($result)) {
            echo "<tr>";
            echo "<td>" . $row['FacilityID'] . "</td>";
            echo "<td>" . $row['MinistryID'] . "</td>";
            echo "<td>" . $row['Name'] . "</td>";
            echo "<td>" . $row['Address'] . "</td>";
            echo "<td>" . $row['City'] . "</td>";
            echo "<td>" . $row['Province'] . "</td>";
            echo "<td>" . $row['CapOnEmployees'] . "</td>";
            echo "<td>" . $row['PostalCode'] . "</td>";
            echo "<td>" . $row['Phone'] . "</td>";
            echo "<td>" . $row['WebAddress'] . "</td>";
            echo "<td>" . $row['Type'] . "</td>";
            echo "</tr>";
        }
        echo "</table>";
    } else {
        echo "No facilities found in the database.";
    }
    mysqli_close($conn);
}

