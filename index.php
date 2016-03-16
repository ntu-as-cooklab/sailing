<?php

$command = escapeshellcmd('./nc-read');
$output = shell_exec($command);
echo $output;

?>
