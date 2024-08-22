<?php

function console_log($output, $with_script_tags = true) {
    $js_code = 'console.log(' . json_encode($output, JSON_HEX_TAG) .
        ');';
    if ($with_script_tags) {
        $js_code = '<script>' . $js_code . '</script>';
    }
    echo $js_code;
}

console_log("lol");

require("../php/util/user_reader.php");
console_log($_POST);

if (!empty($_POST)) {
    console_log("1");
    $email = $_POST['email'];

    if (file_exists("../database/users.json")) {
        $input = file_get_contents("../database/users.json", true);
        $users = json_decode($input);
        console_log("2");
    }

    if (!isset_user($email)) {
        console_log("3");
        $thisUser = array(
            "firstName" => $_POST['firstName'],
            "lastName" => $_POST['lastName'],
            "email" => $email,
            "address" => $_POST['address'],
            "postalCode" => $_POST['postalCode'],
            "password" => $_POST['password'],
            "role" => $_POST['role'],
        );

        $users[] = $thisUser;

        $json = json_encode($users);

        //write json to file

        if (file_put_contents("../database/users.json", $json)) {
            console_log("4");
            echo "<script type='text/javascript'> document.location = 'login.php'; </script>";
        } else {
            console_log("5");
            echo "Oops! Error creating json file...";
        }
    } else {
        console_log("6");
        echo "<script type='text/javascript'> document.location = 'sign_up.html?msg=This User Already Exists'; </script>";
    }
    console_log("7");

}
console_log("8");
