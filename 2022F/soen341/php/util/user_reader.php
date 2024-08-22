<?php

/**
 * Checks whether a user exists.
 *
 * @param $email e-mail of the user
 * @return bool whether the user has an account
 */
function isset_user($email)
{
    $exists = false;
    $json_array = get_users_json_array();

    foreach ($json_array as $user)
        if (strcasecmp($user["email"], $email) == 0)
            $exists = true;

    return $exists;
}

function authenticate($email, $pwd)
{
    $authed = false;
    $json_array = get_users_json_array();

    foreach ($json_array as $user) {
        if (strcasecmp($user["email"], $email) == 0 and strcasecmp($user["password"], $pwd) == 0)
            $authed = true;
    }

    return $authed;
}

function get_user_property($email, $property_name)
{
    $json_array = get_users_json_array();

    foreach ($json_array as $user)
        if (strcasecmp($user["email"], $email) == 0)
            return $user[$property_name];

    return null;
}

function get_users_json_array()
{
    $json_str_contents = file_get_contents("../database/users.json", true);
    return json_decode($json_str_contents, true);
}