require 'net/http'

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
  param: {
    mode: 'stop',
    stop: ["2208"],
  }
}
uri = URI(url)
uri.query = URI.encode_www_form(query)
response = Net::HTTP.get_response(uri)
puts response.body
exit
doc = Nokogiri::XML.parse(page.body)

node = doc.css("departure").first
time = Time.parse(node.text)
p time

