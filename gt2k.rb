require 'net/http'
require 'time'
require 'io/wait'

def get_delay
  url = 'http://data.keolis-rennes.com/xml/'
  query = {
    cmd: 'getbusnextdepartures',
    version: '2.1',
    key: 'RK8BUNJ7NSYTNFU',
    #param: {
    #  mode: 'stopline',
    #  route: [0077],
    #  direction: [0],
    #  stop: [3115],
    #}
    "param[mode]" => 'stop',
    "param[stop][]" => "1052",
  }
  uri = URI(url)
  uri.query = URI.encode_www_form(query)
  response = Net::HTTP.get_response(uri)

  #puts response.body

  if response.body =~ /localdatetime="(.+?)"/
    now = Time.parse($1)
  else
    puts response.body
    raise "no local time"
  end

  if response.body =~ /<departure.*?>(.+?)<\/departure>/
    time = $1
    puts time
    time = Time.parse(time)
  else
    puts response.body
    raise "no time"
  end
  time - now
end

$serial = File.open("/dev/ttyUSB0", "w+")

def set_color(r,g,b)
  command = [r,g,b].join(",")
  p command
  $serial.syswrite(command + "\n")
end

def button_pressed?
  while $serial.ready?
    if $serial.gets.strip == "Button_pressed"
      return true
    end
  end
end

Infinity = Float::INFINITY

def play(name)
  system "madplay /music/#{name} &"
end

def run_alarm(walk_minutes = 5)
  last_update = nil
  arrival = nil
  minutes_before = nil
  loop do
    return if button_pressed?

    now = Time.now
    if last_update.nil? or now >= last_update + 30
      puts "update"
      arrival = now + get_delay
      last_update = now
    end

    minutes = (arrival - now) / 60.0
    p bus_minutes: minutes
    minutes += walk_minutes

    if minutes_before
      if minutes.to_i != minutes_before.to_i
        case minutes.round
        when 1 then play "1minute.mp3"
        when 2 then play "2minute.mp3"
        when 3 then play "3minute.mp3"
        when 5 then play "5minute.mp3"
        end
      end
    end
    minutes_before = minutes

    case minutes
    when 20.0..Infinity
      color = [0,0,1]
    when 10.0...20.0
      nr = (minutes - 10.0)/10.0
      r = 1.0 - nr
      color = [0,r,nr]
    when 0.0...10.0
      nr = minutes/10.0
      r = 1.0 - nr
      color = [r, nr, 0]
    when -Infinity..0.0
      color = [1,0,0]
    end
    p color: color

    color = color.map { |c| (c * 255).round }

    set_color(*color)

    sleep 2
  end
end

at_exit do
  set_color(0,0,0)
end

loop do
  set_color(0,0,0)
  #$serial.wait
  if true #button_pressed?
    play("active.mp3")
    set_color(0,0,64)
    run_alarm
  end
end
