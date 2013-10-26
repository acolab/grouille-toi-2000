require 'net/http'
require 'time'

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

puts response.body

if response.body =~ /localdatetime="(.+?)"/
  now = Time.parse($1)
end

if response.body =~ /<departure.*?>(.+?)<\/departure>/
  time = $1
  time = Time.parse(time)
end
remaining = time - now
minutes = remaining / 60.0
puts minutes


