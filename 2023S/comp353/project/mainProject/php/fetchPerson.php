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

// Retrieve the SSN from the form and fetch the details from the database
if(isset($_POST['MedicareCardNumber'])) {
    $MedicareCardNumber = $_POST['MedicareCardNumber'];

    $query = "SELECT firstName, lastName FROM Person WHERE MedicareCardNumber = ?";
    $stmt = mysqli_prepare($conn, $query);

    mysqli_stmt_bind_param($stmt, 's', $MedicareCardNumber);
    mysqli_stmt_execute($stmt);

    $result = mysqli_stmt_get_result($stmt);
    $person = mysqli_fetch_assoc($result);

    if ($person) {
        echo "First Name: " . $person['firstName'] . "<br>";
        echo "Last Name: " . $person['lastName'];
    } else {
        echo "No person found with the provided Medicare Card Number.";
    }

    mysqli_stmt_close($stmt);
} else {
    echo "Medicare Card Number not provided.";
}


/////////////
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

    $query = "SELECT * FROM Person";
    $result = mysqli_query($conn, $query);

    if (mysqli_num_rows($result) > 0) {
        // Display the result as a table
        echo "<table border='1'>";
        echo "<tr><th>MedicareCardNumber</th><th>First Name</th><th>Last Name</th></tr>"; // Add more column headers based on your table structure
        while ($row = mysqli_fetch_assoc($result)) {
            echo "<tr>";
            echo "<td>" . $row['MedicareCardNumber'] . "</td>";
            echo "<td>" . $row['FirstName'] . "</td>";
            echo "<td>" . $row['LastName'] . "</td>";
            echo "</tr>";
        }
        echo "</table>";
    } else {
        echo "No persons found in the database.";
    }
    mysqli_close($conn);
}

?>
