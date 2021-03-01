<?php

    // Turn off error reporting
    error_reporting(0);

    // Report runtime errors
    error_reporting(E_ERROR | E_WARNING | E_PARSE);

    // Report all errors
    error_reporting(E_ALL);

    // Same as error_reporting(E_ALL);
    ini_set("error_reporting", E_ALL);

    // Report all errors except E_NOTICE
    error_reporting(E_ALL & ~E_NOTICE);

    header("Access-Control-Allow-Origin: *");
    header("Content-Type: application/json; charset=UTF-8");
    header("Access-Control-Allow-Methods: POST");
    header("Access-Control-Max-Age: 3600");
    header("Access-Control-Allow-Headers: Content-Type, Access-Control-Allow-Headers, Authorization, X-Requested-With");

    require_once("./db.php");

    $data = array();
    $postdata = file_get_contents("php://input");
    // echo $postdata;
    $request = json_decode($postdata, true);

    // debug
    // $request['id'] = 1;

    if(isset($request['id'])){
        $sql = "SELECT * FROM users WHERE id = '".$request['id']."' ";
        $result = $conn->query($sql);
    
        if ($result->num_rows > 0) {
            // output data of each row
            while($row = $result->fetch_assoc()) {
                $data['name']= $row['name'];
            }
        } else {
            $data['name']= "No math";
        }
        $conn->close();
    
        echo json_encode($data);
    }
   

?>