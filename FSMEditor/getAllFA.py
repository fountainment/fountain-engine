import os

ans = []

wl = os.walk(".")
for root, folder, files in wl:
    for f in files:
        if '.sip' in f:
            ans.append(root + '/' + os.path.splitext(f)[0] + '.png' + ' ' + root + '/' + f)
print len(ans)
for s in ans:
    print('%s' % s)
