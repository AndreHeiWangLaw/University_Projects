<?php
$host = 'fdc353.encs.concordia.ca';
$user = 'fdc353_1';
$pass = '353psswd';
$db = 'fdc353_1';

// Connect to the database
$pdo = new PDO("mysql:host=$host;dbname=$db", $user, $pass);

if(isset($_POST['executeSql'])){
    $sql = "
    SELECT
    F.Name,
    F.Address,
    F.City,
    F.Province,
    F.PostalCode,
    F.Phone,
    F.WebAddress,
    F.Type,
    F.CapOnEmployees AS Capacity,
    CASE
        WHEN F.Type = 'educational' THEN E.principle
        WHEN F.Type = 'management' THEN M.presidentID
        ELSE NULL
    END AS PrincipalOrPresidentID,
    CASE
        WHEN F.Type = 'educational' THEN (SELECT FirstName FROM Person WHERE MedicareCardNumber = E.principle)
        WHEN F.Type = 'management' THEN (SELECT FirstName FROM Person WHERE MedicareCardNumber = M.presidentID)
        ELSE NULL
    END AS FirstName,
    CASE
        WHEN F.Type = 'educational' THEN (SELECT LastName FROM Person WHERE MedicareCardNumber = E.principle)
        WHEN F.Type = 'management' THEN (SELECT LastName FROM Person WHERE MedicareCardNumber = M.presidentID)
        ELSE NULL
    END AS LastName,
    (SELECT COUNT(*) FROM Employee WHERE FacilityID = F.FacilityID) AS NumberOfEmployees
FROM
    Facility F
LEFT JOIN Educational E ON F.FacilityID = E.FacilityID AND F.MinistryID = E.MinistryID
LEFT JOIN Management M ON F.FacilityID = M.FacilityID AND F.MinistryID = M.MinistryID
ORDER BY
    F.Province ASC,
    F.City ASC,
    F.Type ASC,
    NumberOfEmployees ASC;

    ";

    $statement = $pdo->query($sql);
    $results = $statement->fetchAll(PDO::FETCH_ASSOC);

    // Do whatever you need with the results (e.g., display them, store them, etc.)
    echo '<pre>';
    print_r($results);
    echo '</pre>';
}

?>
