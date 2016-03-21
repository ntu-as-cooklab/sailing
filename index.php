<?php

$command = escapeshellcmd('./sailing');
$output = shell_exec($command);
echo $output;

?>
