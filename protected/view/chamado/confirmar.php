<?php 

echo '<legend>input -> POST</legend>';
echo '<pre>';
var_dump($input['post']);
echo '</pre>';
echo '<legend>input -> GET</legend>';
echo '<pre>';
var_dump($input['get']);
echo '</pre>';
echo '<legend>input -> ARGS</legend>';
echo '<pre>';
var_dump($input['args']);
echo '</pre>';
echo '<legend>output response</legend>';
echo '<pre>';
var_dump($response);
echo '</pre>';
echo '<legend>_SERVER</legend>';
echo '<pre>';
var_dump($_SERVER);
echo '</pre>';
echo '<legend>_FILES</legend>';
echo '<pre>';
var_dump($_FILES);
echo '</pre>';