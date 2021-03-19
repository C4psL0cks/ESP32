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

    $target_dir = "uploads/";
    $datum = mktime(date('H') + 0, date('i'), date('s'), date('m'), date('d'), date('y'));
    $target_file = $target_dir . date('d-m-Y_H-i-s_', $datum) . basename($_FILES["imageFile"]["name"]);
    $imageFileType = strtolower(pathinfo($target_file, PATHINFO_EXTENSION));

    // Check if image file is a actual image or fake image
    if (!empty($_FILES["imageFile"]["tmp_name"])) {
        // Allow certain file formats
        $allowTypes = array('jpg', 'png', 'jpeg', 'gif', 'pdf');
        if (in_array($imageFileType, $allowTypes)) {
            // Upload file to server
            if (move_uploaded_file($_FILES["imageFile"]["tmp_name"], $target_file)) {
                echo $statusMsg = "ไฟล์ " . $target_file . " อัพโหลดเสร็จเรียบร้อย.";
            } else {
                echo $statusMsg = "เสียใจ ,การอัพโหลดไฟล์ผิดพลาด.";
            }
        } else {
            echo $statusMsg = 'เสียใจ คุณต้องเลือกภาพที่มีนามสกุล JPG, JPEG, PNG, GIF, & PDF ถึงจะอัพโหลดได้.';
        }
    }

?>