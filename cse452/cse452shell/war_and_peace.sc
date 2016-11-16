background [
	color 1 1 1
]

camera [
	eye -3 1 0
	look 3 -1 0
	up 0 1 0
	angle 45
]

light [
	position 5 5 0
	color 1 1 1
]

light [
	position 3 3 3
	color 1 1 1
]

light [
	position 5 5 -5
	color 1 1 1
]

light [
	position -3 3 5
	color 1 1 1
]

mastersubgraph teapot [
	trans [
		translate 0 0 0
		scale .1 .1 .1
		object ellipsoid [
			diffuse 1 1 1
			reflect 0 1 0
		]
	]

	trans [
		translate 0 .05 0
		scale .05 .05 .05
		object sphere [
			diffuse 1 1 1
			reflect 0 0 1
		]
	]

	trans [
		translate 0 .03 .15
		rotate 1 0 0 75
		scale .03 .12 .03
		object cone [
			diffuse 1 1 1
			reflect 0 0 1
		]
	]

	trans [
		translate 0 .03 -.1
		rotate 1 0 0 90
		scale .02 .07 .02
		object cylinder [
			diffuse 1 1 1
			reflect 0 0 1
		]
	]

	trans [
		translate 0 0 -.125
		scale .02 .05 .02
		object cylinder [
			diffuse 1 1 1
			reflect 0 0 1
		]
	]

	trans [
		translate 0 -.02 -.1
		rotate 1 0 0 90
		scale .02 .06 .02
		object cylinder [
			diffuse 1 1 1
			reflect 0 0 1
		]
	]
]

mastersubgraph go [
	trans [
		translate 0 0 0
		scale .04 .04 .04
		object ellipsoid [
			diffuse 1 1 1
			reflect 0 1 0
		]
	]
	
	trans [
		translate .15 0 .1
		scale .04 .04 .04
		object ellipsoid [
			diffuse 1 1 1
			reflect 0 1 0
		]
	]

	trans [
		translate 0 0 .1
		scale .04 .04 .04
		object ellipsoid [
			diffuse 0 0 0
			reflect 0 1 0
		]
	]
	
	trans [
		translate 0 0 .2
		scale .04 .04 .04
		object ellipsoid [
			diffuse 0 0 0
			reflect 0 1 0
		]
	]

	trans [
		translate -.15 0 .2
		scale .04 .04 .04
		object ellipsoid [
			diffuse 1 1 1
			reflect 0 1 0
		]
	]
]


mastersubgraph root [
	trans [
		translate 0 0 0
		scale 2 .1 2
		object cube [
			diffuse .3 .3 .3
		]
	]

	trans [
		translate -.8 -.4 -.8
		scale .2 .8 .2
		object cylinder [
			diffuse .7 .7 .7
		]
	]

	trans [
		translate .8 -.4 .8
		scale .2 .8 .2
		object cylinder [
			diffuse .7 .7 .7
		]
	]

	trans [
		translate -.8 -.4 .8
		scale .2 .8 .2
		object cylinder [
			diffuse .7 .7 .7
		]
	]
	
	trans [
		translate .8 -.4 -.8
		scale .2 .8 .2
		object cylinder [
			diffuse .7 .7 .7
		]
	]

	trans [
		translate -.7 0.1 -.7
		subgraph teapot
	]

	trans [
		translate .7 0.1 .7
		rotate 0 1 0 180
		subgraph teapot
	]


	trans [
		translate 0 0.1 0
		subgraph go
	]

	trans [
		translate -.3 0.1 -.3
		subgraph go
	]

	trans [
		translate -.3 0.1 .3
		rotate 0 1 0 180
		subgraph go
	]

	trans [
		translate -.7 .1 .7
		scale .2 .2 .2
		object cactus [
			diffuse 1 0 0
		]
	]
	
	trans [
		translate .7 .1 -.7
		scale .2 .2 .2
		object cactus [
			diffuse 0 0 1
		]
	]

	

]