const char WEBPAGE_ROOT[] PROGMEM = R"=====(
 <!DOCTYPE html>
<html>
<head>
  <script src="https://ajax.googleapis.com/ajax/libs/jquery/3.4.1/jquery.min.js"></script>
  <script src="main.js"></script>
</head>
<body>
  <h1>ESP32 Showcontrol unit</h1>

  <h2>Settings</h2>
  <h3>Master QLab machine</h3>
  <form action="/set/master" method="post" name="master_form" id="master_form">
    <caption for="master_name">Name: </caption><input type="text" name="master_name" id="master_name" value="x"/><br/>
    <caption for="master_ip">IP: </caption><input type="text" name="master_ip" id="master_ip" value="x"/><br/>
    <caption for="master_port">Port: </caption><input type="text" name="master_port" id="master_port" value="x"/><br/>
    <input type="button" value="Save" onclick="javascript:document.getElementById('master_form').submit();"/>
  </form>
</body>

</html>
)=====";
