import Queue
import urllib2
import urlparse
import sys
import re
from bs4 import BeautifulSoup

def check(link):
    broken = (link[-3:] in ["jpg", "png", "svg"])
    broken = broken or link.find("index.php?")>-1
    path = urlparse.urlsplit(link)[2]
    # if path[:5] != "/wiki"
    if (broken or path[:5] != "/wiki"): #We have already been on start page (or link is broken)
        return False
    path = path[6:]
    broken = path.find(':')>-1 or path.find('.')>-1
    return not broken

def getPath(link):
    path = urlparse.urlsplit(link)[2]
    path = path[5:]
    return path

def htmlToText(html):
    text = BeautifulSoup(html).text
    text = text[text.find('\n\n\n\n'):text.rfind('\n\n\n\n')]
    text = ' '.join(re.split(r'\d', text))
    text = ' '.join(re.split(r'\W', text))
    while (text.find('  ')>-1):
        text = ' '.join(text.split('  '))
    return text

def newDoc(f, Id, size = 1500):
    new = (Id - 1) % size == 0
    name = str((Id + size - 1) / size) + '.txt'
    if (new):
        f.close()
        f = open(str((Id + size - 1) / size) +'.txt', 'w')
    return f, new, name

starturl = 'http://simple.wikipedia.org/wiki'

queue = Queue.Queue()
queue.put(starturl)
visited = set()
visited.add(getPath(starturl))

dist = dict([(starturl, 0)])
degIN = dict([])
degOUT = dict([])

f = open('1.txt','w')
f_url = open('URLS.txt','w')

urlID = 0

while (not queue.empty() and urlID < int(sys.argv[1])):
    url = queue.get()
    page = urllib2.urlopen(url)
    host = urlparse.urlsplit(url)[1]
    url_dist = dist[url]
    urlID += 1
    # save pair (urlID.html -> url) in file "urls.txt"
    
    if (urlID % 50 == 0):
        print urlID, url, queue.qsize()
    
    html = page.read()
    ####download html in to "urlID.html'.####
    #########################################
    f, newFile, fileName = newDoc(f, urlID)
    f.write(htmlToText(html) + ' ')
    if (newFile):
        f_url.write('\n' + fileName + '\t')
    f_url.write(url + ';')
    #########################################
    #modify "urlID.html" to "urlID.txt" with deleting all other symbols
    
    soup = BeautifulSoup(html)
    cnt_links = 0
    for tag in soup.findAll('a', href = True):
        link = tag['href']
        link = urlparse.urljoin(url, link) #in case of the local-link add global path
        link = link.split('#')[0] # delete scrolling on page
        
        host_link = urlparse.urlsplit(link)[1]
        if (host_link == host and check(link)):# check whether link is inner link and correct
            if (degIN.get(link,-1)==-1):
                degIN[link] = 0
            degIN[link] += 1
            cnt_links += 1
            
            if (getPath(link) not in visited): #check whether link was visited prev.
                visited.add(getPath(link))
                queue.put(link)
                dist[link] = 1 + url_dist
    degOUT[url] = cnt_links
    
f_url.close()
f.close()
