import shutil, re, math
import xml.etree.ElementTree as ET
from pathlib import Path

def calc_distance(origin, destination):
    '''Calculates the distance between two pairs of (lat, lon)'''
    '''Taken from https://gist.github.com/rochacbruno/2883505 '''
    lat1, lon1 = origin
    lat2, lon2 = destination
    radius = 6371 # km
    dlat = math.radians(lat2-lat1)
    dlon = math.radians(lon2-lon1)
    a = math.sin(dlat/2) * math.sin(dlat/2) + math.cos(math.radians(lat1)) \
        * math.cos(math.radians(lat2)) * math.sin(dlon/2) * math.sin(dlon/2)
    c = 2 * math.atan2(math.sqrt(a), math.sqrt(1-a))
    d = radius * c

    return d

def remove_namespace(doc, namespace):
    ns = u'{%s}' % namespace
    nsl = len(ns)
    for elem in doc.getiterator():
        if elem.tag.startswith(ns):
            elem.tag = elem.tag[nsl:]


def grab_xml_info(fileName):
    #grab file and copy it to .xml file for easier parsing
    new_filename = Path(fileName).with_suffix('.xml')
    shutil.copy2(fileName, new_filename)
    tree = ET.parse(new_filename)
    root = tree.getroot()
    #remove namespaces which are found in some gpx files
    result = re.search('{(.*)}', root.tag)
    remove_namespace(root, result.group(1))
    tree.write(new_filename)
    #return all of the points wrapped in the <rte> tag,
    #which contain lat/lon and elevation
    points = root.find('rte')
    return points