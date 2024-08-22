<!DOCTYPE html>
<html>
<head>
    <title>Student Registration</title>
</head>
<body>
    <form action="6Processing.php" method="post">
        <label for="operation">Choose an operation:</label>
        <select name="operation" id="operation">
            <option value="register">Register</option>
            <option value="modify">Modify</option>
            <option value="cancel">Cancel</option>
        </select><br><br>
        
        <label for="MedicareCardNumber">Medicare Card Number:</label>
        <input type="number" name="MedicareCardNumber" required><br><br>

        <label for="FacilityID">Facility ID:</label>
        <input type="number" name="FacilityID" required><br><br>

        <label for="MinistryID">Ministry ID:</label>
        <input type="number" name="MinistryID" required><br><br>

        <label for="Level">Level:</label>
        <input type="text" name="Level"><br><br>
        
        <input type="submit" value="Submit">
    </form>
</body>
</html>
