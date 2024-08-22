<?php




if ($_SERVER["REQUEST_METHOD"] == "POST") {
    $dateOfInfection = $_POST["dateOfInfection"];
    $medicareCardNumber = $_POST["medicareCardNumber"];
    $infectionType = $_POST["infectionType"];

    // Perform validation here before executing the SQL query

    $sql = "INSERT INTO Infection (DateOfInfection, MedicareCardNumber, InfectionType) 
            VALUES (?, ?, ?)";
    $stmt = $conn->prepare($sql);
    $stmt->bind_param("sis", $dateOfInfection, $medicareCardNumber, $infectionType);
    $stmt->execute();
    $stmt->close();

    // Redirect to a success page or refresh the current page
    header("Location: vaccinationPage.php");
}
?>
<!-- HTML Form for creating an infection record -->
<form method="post" action="">
    Date of Infection: <input type="date" name="dateOfInfection"><br>
    Medicare Card Number: <input type="number" name="medicareCardNumber"><br>
    Infection Type: <input type="text" name="infectionType"><br>
    <input type="submit" value="Create">
</form>
