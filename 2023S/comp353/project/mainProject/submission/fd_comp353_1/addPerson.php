<?php

$host = 'fdc353.encs.concordia.ca';
$user = 'fdc353_1';
$pass = '353psswd';
$db = 'fdc353_1';

$conn = new mysqli($host, $user, $pass, $db);

if ($conn->connect_error) {
    die("Connection failed: " . $conn->connect_error);
}

if ($_SERVER["REQUEST_METHOD"] == "POST") {
    // Escape and sanitize inputs
    $medicareCardNumber = $conn->real_escape_string($_POST["medicare_card_number"]);
    $firstName = $conn->real_escape_string($_POST["first_name"]);
    $lastName = $conn->real_escape_string($_POST["last_name"]);
    $dateOfBirth = $conn->real_escape_string($_POST["date_of_birth"]);
    $address = $conn->real_escape_string($_POST["address"]);
    $city = $conn->real_escape_string($_POST["city"]);
    $province = $conn->real_escape_string($_POST["province"]);
    $postalCode = $conn->real_escape_string($_POST["postal_code"]);
    $citizenship = $conn->real_escape_string($_POST["citizenship"]);
    $emailAddress = $conn->real_escape_string($_POST["email_address"]);
    $telephoneNumber = $conn->real_escape_string($_POST["telephone_number"]);
    $medicareExpirationDate = $conn->real_escape_string($_POST["medicare_expiration_date"]);
    // ... (other fields)

    $role = $_POST["role"];

    $sql = "INSERT INTO Person (MedicareCardNumber,FirstName, LastName, DateOfBirth, Address, City, Province, PostalCode, Citizenship, EmailAddress, TelephoneNumber, MedicareExpirationDate) 
    VALUES ('$medicareCardNumber','$firstName', '$lastName', '$dateOfBirth', '$address', '$city', '$province', '$postalCode', '$citizenship', '$emailAddress', '$telephoneNumber', '$medicareExpirationDate')";

    if ($conn->query($sql) === TRUE) {
        $personID = $conn->insert_id;

        if ($role == "student") {
            $level = $conn->real_escape_string($_POST["level"]);
            $facilityID = $conn->real_escape_string($_POST["facility_id"]);
            $ministryID = $conn->real_escape_string($_POST["ministry_id"]);

            $studentSql = "INSERT INTO Student (MedicareCardNumber, FacilityID, MinistryID, Level) VALUES ('$medicareCardNumber', '$facilityID', '$ministryID', '$level')";
            $conn->query($studentSql);
        } elseif ($role == "teacher") {
            $facilityID = $conn->real_escape_string($_POST["facility_id"]);
            $ministryID = $conn->real_escape_string($_POST["ministry_id"]);

            $employeeSql = "INSERT INTO Employee (Role, MedicareCardNumber, FacilityID, MinistryID) VALUES ('$role', '$medicareCardNumber', '$facilityID', '$ministryID')";
            $conn->query($employeeSql);
        }

        echo "Person added successfully!";
    } else {
        echo "Error: " . $sql . "<br>" . $conn->error;
    }
}

$conn->close();
?>
