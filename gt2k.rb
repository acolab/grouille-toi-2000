require 'bundler/setup'

require 'httparty'

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
    stop: ["2208", "3216"],
  }
}
page = HTTParty.get(url, query: query)

puts page.body

