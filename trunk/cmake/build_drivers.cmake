ADD_CUSTOM_COMMAND ( 
  OUTPUT hello.ko
  COMMAND cd /home/vijay/toobo/toobo/drivers/modules/hello-2.6/ && make
  )

ADD_CUSTOM_COMMAND ( 
  OUTPUT graphiclcd.ko
  COMMAND cd /home/vijay/toobo/toobo/drivers/modules/glcd-2.6/ && make
  )

ADD_CUSTOM_COMMAND ( 
  OUTPUT hello.ko
  COMMAND cd /home/vijay/toobo/toobo/drivers/modules/hello-2.6/ && make
  )

ADD_CUSTOM_COMMAND ( 
  OUTPUT graphiclcd.ko
  COMMAND cd /home/vijay/toobo/toobo/drivers/modules/glcd-2.6/ && make
  )

ADD_CUSTOM_TARGET (drivers ALL DEPENDS hello.ko graphiclcd.ko)

