<?php
if ($_SERVER["REQUEST_METHOD"] == "POST") {
    $newInfectionType = $_POST["newInfectionType"];
    $medicareCardNumber = $_POST["medicareCardNumber"];
    $dateOfInfection = $_POST["dateOfInfection"];

    $sql = "UPDATE Infection SET InfectionType = ? WHERE MedicareCardNumber = ? AND DateOfInfection = ?";
    $stmt = $conn->prepare($sql);
    $stmt->bind_param("sis", $newInfectionType, $medicareCardNumber, $dateOfInfection);
    $stmt->execute();
    $stmt->close();

    // Redirect to a success page or refresh the current page
    header("Location: success.php");
}
?>
<!-- HTML Form for editing an infection record -->
<form method="post" action="">
    New Infection Type: <input type="text" name="newInfectionType"><br>
    Medicare Card Number: <input type="number" name="medicareCardNumber"><br>
    Date of Infection: <input type="date" name="dateOfInfection"><br>
    <input type="submit" value="Edit">
</form>
