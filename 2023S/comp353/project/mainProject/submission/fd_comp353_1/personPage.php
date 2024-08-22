<!DOCTYPE html>
<html>
<head>
    <title>Retrieve Person Information</title>
</head>
<body>
    <form action="fetchPerson.php" method="post">
        Enter MedicareCardNumber to find person: <input type="text" name="MedicareCardNumber">
        <input type="submit" value="Fetch Details">
    </form>

    <form action="fetchPerson.php" method="post">
    <input type="submit" name="fetch_all" value="Fetch All Persons">
</form>

<h2>Add a Person</h2>
    <form action = "addPerson.php" method="post">
        <!-- Input fields for person details -->
        <!-- ... (other fields) -->
        <label for="medicare_card_number">Medicare Card Number:</label>
<input type="text" name="medicare_card_number" required><br>

        <label for="first_name">First Name:</label>
        <input type="text" name="first_name" required><br>
        
        <label for="last_name">Last Name:</label>
        <input type="text" name="last_name" required><br>
        
        <label for="date_of_birth">Date of Birth:</label>
        <input type="date" name="date_of_birth" required><br>
        
        <label for="address">Address:</label>
        <input type="text" name="address" required><br>
        
        <label for="city">City:</label>
        <input type="text" name="city" required><br>
        
        <label for="province">Province:</label>
        <input type="text" name="province" required><br>
        
        <label for="postal_code">Postal Code:</label>
        <input type="text" name="postal_code" required><br>
        
        <label for="citizenship">Citizenship:</label>
        <input type="text" name="citizenship" required><br>
        
        <label for="email_address">Email Address:</label>
        <input type="email" name="email_address" required><br>
        
        <label for="telephone_number">Telephone Number:</label>
        <input type="tel" name="telephone_number" required><br>
        
        <label for="medicare_expiration_date">Medicare Expiration Date:</label>
        <input type="date" name="medicare_expiration_date" required><br>

        <label for="role">Role:</label>
        <select name="role" required>
            <option value="student">Student</option>
            <option value="teacher">Teacher</option>
        </select><br>
        
        <label for="level">Level (if student):</label>
        <input type="text" name="level"><br>
        
        <label for="facility_id">Facility ID (if student or teacher):</label>
        <input type="text" name="facility_id"><br>
        
        <label for="ministry_id">Ministry ID (if student or teacher):</label>
        <input type="text" name="ministry_id"><br>
        
        <input type="submit" value="Add Person">
    </form>
</body>
</html>
