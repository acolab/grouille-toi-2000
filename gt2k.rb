require 'net/http'
require 'time'
require 'io/wait'

def get_delay
  return 3*60
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
    "param[stop][]" => "2208",
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

Thread.new do
  serial = File.open("/dev/ttyUSB0", "r")
  loop do
    p serial.sysread(100)
  end
  serial.close
end

def set_color(r,g,b)
  serial = File.open("/dev/ttyUSB0", "w")
  command = [r,g,b].join(",")
  p command
  serial.syswrite(command + "\n")
  serial.close
end

def button_pressed?
  true
end

Infinity = Float::INFINITY

def run_alarm
  last_update = nil
  arrival = nil
  loop do
    now = Time.now
    if last_update.nil? or now >= last_update + 30
      puts "update"
      arrival = now + get_delay
      last_update = now
    end

    minutes = (arrival - now) / 60.0
    p minutes: minutes

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

loop do
  if button_pressed?
    run_alarm
  end
end
