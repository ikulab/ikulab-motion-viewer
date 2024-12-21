import json
import urllib.request

def get_latest_release_version():
    # GitHub APIのURL
    url = "https://api.github.com/repos/ikulab/ikulab-motion-viewer/releases/latest"

    req = urllib.request.Request(url)
    with urllib.request.urlopen(req) as res:
        body = res.read()
        json_body = json.loads(body)
        return json_body["tag_name"]
