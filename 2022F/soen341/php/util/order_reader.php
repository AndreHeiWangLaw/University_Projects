<?php

function isset_id($path, $id)
{
    $json_array = get_orders_json_array($path);

    foreach ($json_array as $order)
        if ($order["id"] == $id)
            return true;

    return false;
}

function get_property_by_id($path, $id, $property)
{
    $json_array = get_orders_json_array($path);

    foreach ($json_array as $order)
        if ($order["id"] == $id)
            return $order[$property];
}

function get_products_by_id($path, $id)
{
    $json_array = get_orders_json_array($path);

    foreach ($json_array as $order)
        if ($order["id"] == $id)
            return $order["products"];

    return null;
}

function get_orders_json_array($path)
{
    $json_str_contents = file_get_contents($path, true);
    return json_decode($json_str_contents, true);
}
