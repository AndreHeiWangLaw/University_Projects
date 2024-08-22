<?php
if (isset($_GET["delete"]) && isset($_GET["medicareCardNumber"]) && isset($_GET["dateOfInfection"])) {
    $medicareCardNumber = $_GET["medicareCardNumber"];
    $dateOfInfection = $_GET["dateOfInfection"];

    $sql = "DELETE FROM Infection WHERE MedicareCardNumber = ? AND DateOfInfection = ?";
    $stmt = $conn->prepare($sql);
    $stmt->bind_param("is", $medicareCardNumber, $dateOfInfection);
    $stmt->execute();
    $stmt->close();

    // Redirect to a success page or refresh the current page
    header("Location: success.php");
}
?>
<!-- Link for deleting an infection record -->
<a href="delete_infection.php?delete=true&medicareCardNumber=234567890&dateOfInfection=2023-01-25">Delete Infection</a>
