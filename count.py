import csv
import os
from pprint import pprint

if 'writeheader' not in dir(csv.DictWriter):
    def writeheader(self):
        header = dict(zip(self.fieldnames, self.fieldnames))
        self.writerow(header)
    csv.DictWriter.writeheader = writeheader


out_dir='charts/data'
if not os.path.exists(out_dir): os.makedirs(out_dir)

for file in os.listdir("out"):
    with open(file, 'rb') as f:
        reader = csv.reader(f)
        headers = map(lambda name: name.strip(), reader.next())
 
        lines = []
        for row in reader:
            lines.append(dict(zip(headers, map(lambda value: float(value) if "." in value else int(value), row))))

	line['problem_size']=int(file.replace("out_").replace(".txt"))

# print lines
sequenceTimes = dict((line['problem_size'], line) for line in lines if line['procs'] == 1)
# pprint(sequenceTimes)

for line in lines:
    line['speedup'] = sequenceTimes[line['problem_size']]['time'] / line['time']
    line['efficiency'] = line['speedup'] / line['procs']
    if (line['procs'] > 1):
        line['karp_flatt'] = (1.0 / line['speedup'] - 1.0 / line['procs'] ) / (1.0 - 1.0 / line['procs'])
    else:
        line['karp_flatt'] = None
    
    line['speedup_scal'] = line['problem_size'] * sequenceTimes[1]['time'] / line['time']
    line['efficiency_scal'] = line['speedup_scal'] / line['procs']
    if (line['procs'] > 1):
        line['karp_flatt_scal'] = (1.0 / line['speedup_scal'] - 1.0 / line['procs'] ) / (1.0 - 1.0 / line['procs'])
    else:
        line['karp_flatt_scal'] = None

with open(out_dir + '/counted.csv', 'w') as csvfile:
    fieldnames = headers + ['speedup', 'efficiency', 'karp_flatt', 'speedup_scal', 'efficiency_scal', 'karp_flatt_scal']
    writer = csv.DictWriter(csvfile, fieldnames=fieldnames, delimiter=',', lineterminator='\n')
    writer.writeheader()
    writer.writerows(lines)

by_size = {}
for line in lines:
    if line['problem_size'] not in by_size:
        by_size[line['problem_size']] = []
    by_size[line['problem_size']].append(line)

for (size, rows) in by_size.iteritems():
    with open(out_dir + '/counted_%s.csv'%size, 'w') as csvfile:
        fieldnames = headers + ['speedup', 'efficiency', 'karp_flatt', 'speedup_scal', 'efficiency_scal', 'karp_flatt_scal']
        writer = csv.DictWriter(csvfile, fieldnames=fieldnames, delimiter=',', lineterminator='\n')
        writer.writeheader()
        writer.writerows(rows)
