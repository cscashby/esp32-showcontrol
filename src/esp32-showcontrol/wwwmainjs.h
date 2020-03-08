const char MAINJS_ROOT[] PROGMEM = R"=====(
$(document).ready(function() {
  $.ajax({
    url: "/get",
    type: 'GET',
    cache: false,
    success: function(result) {
        if (result) {
          $('#master_name').val(result.network.master_host.name);
          $('#master_ip').val(result.network.master_host.host);
          $('#master_port').val(result.network.master_host.port);
        } else {
          alert("Didn't get default config");
        }
    },
    error: function() {
        alert("No");
    }
  });
});
)=====";
